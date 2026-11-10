// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/garbage-collection/. The authors are listed in prov/NEW-garbage-collection.jsonl.
import { scene } from "@prog/viz";
import type { GraphView, GraphNode, GraphEdge } from "@prog/viz";

interface Obj {
  id: string;
  x?: number;
  y?: number;
}

interface Root {
  name: string;
  kind: "stack" | "global";
  to: string | null;
  y?: number;
}

interface Input {
  objects: (string | Obj)[];
  refs: [string, string][];
  roots: Root[];
  // drop names the roots that the program sets to nil before the collection. Reference counting runs for each of them.
  drop?: string[];
}

type Colour = "white" | "grey" | "black";

// list writes ids as "A", "A and B" or "A, B and C".
function list(xs: string[]): string {
  if (xs.length === 0) return "none";
  if (xs.length === 1) return xs[0];
  return xs.slice(0, -1).join(", ") + " and " + xs[xs.length - 1];
}

// slot names a root, such as "the stack slot x" or "the global g".
function slot(r: Root): string {
  return (r.kind === "stack" ? "the stack slot " : "the global ") + r.name;
}

function cap(s: string): string {
  return s.charAt(0).toUpperCase() + s.slice(1);
}

export default scene({
  slug: "garbage-collection",
  input: { type: "heap: objects, references, roots and the roots to drop", length: [1, 12] },
  run(v, input: Input) {
    // Read and check the input.
    const objs: Obj[] = input.objects.map((o) => (typeof o === "string" ? { id: o } : { ...o }));
    if (objs.length < 1 || objs.length > 12) throw new Error("The heap must have 1 to 12 objects.");
    const roots: Root[] = input.roots.map((r) => ({ ...r }));
    if (roots.length < 1 || roots.length > 4) throw new Error("The program must have 1 to 4 roots.");
    const ids = objs.map((o) => o.id);
    const names = new Set<string>();
    for (const n of [...ids, ...roots.map((r) => r.name)]) {
      if (typeof n !== "string" || n === "") throw new Error("Each object and each root needs a name.");
      if (names.has(n)) throw new Error(`The name ${n} is used twice.`);
      names.add(n);
    }
    for (const r of roots) {
      if (r.kind !== "stack" && r.kind !== "global") throw new Error(`The root ${r.name} must be a stack slot or a global.`);
      if (r.to !== null && !ids.includes(r.to)) throw new Error(`The root ${r.name} refers to ${r.to}, which is not an object.`);
    }
    let refs: [string, string][] = [];
    for (const [a, b] of input.refs) {
      if (!ids.includes(a) || !ids.includes(b)) throw new Error(`The reference ${a} to ${b} names an object that does not exist.`);
      if (a === b) throw new Error(`The object ${a} cannot refer to itself in this scene.`);
      if (refs.some(([p, q]) => p === a && q === b)) throw new Error(`The reference ${a} to ${b} is there twice.`);
      refs.push([a, b]);
    }
    const drop = input.drop ?? [];
    for (const d of drop) {
      const r = roots.find((x) => x.name === d);
      if (!r) throw new Error(`The program cannot drop ${d}, because it is not a root.`);
      if (r.to === null) throw new Error(`The root ${d} is nil already.`);
    }

    // The place of each node: the roots on the left, the objects on the right. The roots keep room on their left for the tag "global".
    const place = new Map<string, [number, number]>();
    roots.forEach((r, i) => place.set(r.name, [0.14, r.y ?? (roots.length === 1 ? 0 : i / (roots.length - 1))]));
    objs.forEach((o, i) => place.set(o.id, [o.x ?? 0.5 + 0.19 * (i % 3), o.y ?? Math.floor(i / 3) / Math.max(1, Math.ceil(objs.length / 3) - 1)]));

    // The state of the heap.
    let live = ids.slice();
    const colour = new Map<string, Colour>();
    let mode: "count" | "colour" = "count";
    const freed: string[] = [];
    let grey: string[] = [];

    const count = (id: string) => roots.filter((r) => r.to === id).length + refs.filter(([, b]) => b === id).length;
    const outs = (id: string) => refs.filter(([a]) => a === id).map(([, b]) => b);
    const reachable = (): Set<string> => {
      const seen = new Set<string>();
      const q = roots.map((r) => r.to).filter((x): x is string => x !== null);
      while (q.length) {
        const x = q.shift()!;
        if (seen.has(x)) continue;
        seen.add(x);
        q.push(...outs(x));
      }
      return seen;
    };
    const tag = (id: string) => (mode === "count" ? `count ${count(id)}` : colour.get(id) ?? "white");

    // draw makes the graph again from the live objects. A freed object is gone from the picture. The player keeps the place of each node.
    let g: GraphView;
    const draw = () => {
      const nodes: GraphNode[] = [
        ...roots.map((r) => ({ id: r.name, label: r.name, x: place.get(r.name)![0], y: place.get(r.name)![1] })),
        ...live.map((id) => ({ id, label: id, x: place.get(id)![0], y: place.get(id)![1] })),
      ];
      const edges: GraphEdge[] = [
        ...roots.filter((r) => r.to !== null).map((r): GraphEdge => [r.name, r.to!]),
        ...refs.map((e): GraphEdge => [e[0], e[1]]),
      ];
      g = v.graph("heap", { nodes, edges, directed: true, layout: "fixed", label: "Roots (left) and heap objects" });
      for (const r of roots) g.nodeLabel(r.name, r.to === null ? "nil" : r.kind);
      for (const id of live) {
        g.nodeLabel(id, tag(id));
        const c = colour.get(id);
        if (c === "grey") g.mark(id, "frontier");
        if (c === "black") g.mark(id, "visited");
      }
      g.frontier(grey, "grey");
    };
    const setColour = (id: string, c: Colour) => {
      colour.set(id, c);
      g.nodeLabel(id, c);
      if (c === "white") g.unmark(id);
      if (c === "grey") g.mark(id, "frontier");
      if (c === "black") g.mark(id, "visited");
    };
    const free = (id: string) => {
      live = live.filter((x) => x !== id);
      refs = refs.filter(([a, b]) => a !== id && b !== id);
      colour.delete(id);
      freed.push(id);
      draw();
      v.vars({ freed: list(freed) });
    };

    // Reference counting, for comparison: the program drops roots before the collection.
    let first = true;
    const dropRef = (y: string, cause: string, remove: () => void) => {
      const before = count(y);
      remove();
      const c = count(y);
      const intro = first ? " Each object keeps a count of the references to it." : "";
      first = false;
      v.line("L1", `${cause}${intro} The count of ${y} goes down from ${before} to ${c}.`);
      v.vars({ x: y });
      draw();
      if (c > 0) {
        const from = [...roots.filter((r) => r.to === y).map(slot), ...refs.filter(([, b]) => b === y).map(([a]) => a)];
        const lost = !reachable().has(y);
        v.line("L2", `The count of ${y} is ${c}, not 0, so ${y} stays in memory.` + (lost ? ` No root reaches ${y} now, but ${list(from)} still refers to it.` : ""));
        return;
      }
      v.line("L2", `The count of ${y} is 0. Nothing refers to ${y} now.`);
      const ys = outs(y);
      if (ys.length === 0) v.line("L3", `${y} refers to no object.`);
      for (const z of ys) {
        v.line("L3", `${y} refers to ${z}. This reference goes away together with ${y}.`);
        g.edge(y, z);
        dropRef(z, `Remove the reference from ${y} to ${z}.`, () => {
          refs = refs.filter(([a, b]) => !(a === y && b === z));
        });
      }
      v.line("L4", `Free ${y}. Reference counting gives the memory back at once.`);
      free(y);
    };
    if (drop.length) v.phase("Reference counting");
    for (const d of drop) {
      const r = roots.find((x) => x.name === d)!;
      const y = r.to!;
      dropRef(y, `The program sets ${slot(r)} to nil.`, () => {
        r.to = null;
      });
    }

    // Tracing: the mark phase.
    v.phase("Mark");
    mode = "colour";
    for (const id of live) colour.set(id, "white");
    const rootWords = list(roots.map(slot));
    v.line(
      "L5",
      (drop.length ? "The collection starts. It does not use the counts." : `The roots are ${rootWords}. The heap has ${live.length} objects. The collection starts.`) +
        " Each object becomes white. White means that the collector has not reached the object yet.",
    );
    v.vars({ x: "none", freed: list(freed) });
    draw();
    for (const r of roots) {
      v.line("L6", r.to === null ? `${cap(slot(r))} is a root, but it is nil.` : `${cap(slot(r))} is a root. It refers to ${r.to}.`);
      g.visit(r.name);
      if (r.to === null) {
        v.line("L7", `${cap(slot(r))} refers to no object, so nothing changes.`);
        continue;
      }
      g.edge(r.name, r.to);
      const c = colour.get(r.to)!;
      if (c === "white") {
        v.line("L7", `${r.to} is white, so ${r.to} becomes grey. Grey means reached, but its references are not scanned yet.`);
        grey.push(r.to);
        setColour(r.to, "grey");
        g.frontier(grey, "grey");
      } else {
        v.line("L7", `${r.to} is ${c} already, so nothing changes.`);
      }
    }
    while (grey.length) {
      v.line("L8", `The grey list holds ${list(grey)}. So the mark phase goes on.`);
      const x = grey.shift()!;
      v.line("L9", `Take ${x} from the grey list and scan its references.`);
      v.vars({ x });
      g.visit(x);
      g.frontier(grey, "grey");
      const ys = outs(x);
      if (ys.length === 0) v.line("L10", `${x} refers to no object.`);
      for (const y of ys) {
        v.line("L10", `${x} refers to ${y}.`);
        g.edge(x, y);
        const c = colour.get(y)!;
        if (c === "white") {
          v.line("L11", `${y} is white, so ${y} becomes grey and goes on the grey list.`);
          grey.push(y);
          setColour(y, "grey");
          g.frontier(grey, "grey");
        } else {
          v.line("L11", `${y} is ${c} already, so nothing changes.`);
        }
      }
      v.line("L12", `${x} becomes black. Black means reached and scanned. No black object refers to a white object.`);
      setColour(x, "black");
    }
    const white = live.filter((id) => colour.get(id) === "white");
    v.line(
      "L8",
      "No object is grey, so the mark phase ends. " +
        (white.length ? `The white objects are ${list(white)}. No path from a root leads to them.` : "No object is white, so each object is reachable."),
    );
    v.vars({ x: "none" });

    // Reference counting alone keeps the white objects that a cycle holds: take away each white object with a count of 0, again and again, and look at what is left.
    const sweepFrom = new Map(live.map((id) => [id, refs.filter(([, b]) => b === id).map(([a]) => a)] as [string, string[]]));
    const stuck = new Set(white);
    for (let changed = true; changed; ) {
      changed = false;
      for (const id of [...stuck]) {
        if (refs.filter(([a, b]) => b === id && stuck.has(a)).length === 0) {
          stuck.delete(id);
          changed = true;
        }
      }
    }

    // Tracing: the sweep phase.
    v.phase("Sweep");
    const freedHere: string[] = [];
    for (const x of live.slice()) {
      v.line("L13", `The sweep looks at ${x}.`);
      v.vars({ x });
      g.visit(x);
      if (colour.get(x) === "white") {
        const from = sweepFrom.get(x)!;
        const why = stuck.has(x) ? ` Its count is still ${from.length}, from ${list(from)}, so reference counting alone would never free it.` : "";
        v.line("L14", `${x} is white. No root reaches it, so the sweep frees ${x}.${why}`);
        free(x);
        freedHere.push(x);
      } else {
        v.line("L14", `${x} is black, so ${x} stays.`);
        v.line("L15", `${x} becomes white again, ready for the next collection.`);
        setColour(x, "white");
      }
    }
    const leak = [...stuck];
    v.line(
      "L13",
      `The sweep has looked at each object, and the collection ends. It freed ${list(freedHere)}.` +
        (leak.length ? ` Reference counting alone would leak ${list(leak)}, because a cycle keeps their counts above 0.` : ""),
    );
    v.vars({ x: "none" });
    return { freed, live };
  },
});
