```typescript run id=engine
// Values carry their type: a discriminated union with a tag.
type Value = { kind: "int"; n: number } | { kind: "float"; x: number };
const int = (n: number): Value => ({ kind: "int", n });
const float = (x: number): Value => ({ kind: "float", x });
const show = (v: Value): string => String(v.kind === "int" ? v.n : v.x);

// The body of f(x) = x * x + 1 as a tree.
type Op = "+" | "*";
type Node =
  | { type: "num"; value: number }
  | { type: "arg" }
  | { type: "bin"; op: Op; left: Node; right: Node };

const OPS: Record<Op, (a: number, b: number) => number> = {
  "+": (a, b) => a + b,
  "*": (a, b) => a * b,
};

const asNumber = (v: Value): number => (v.kind === "int" ? v.n : v.x);

// Generic arithmetic: check both tags on every operation.
function arith(op: Op, a: Value, b: Value): Value {
  if (a.kind === "int" && b.kind === "int") return int(OPS[op](a.n, b.n));
  return float(OPS[op](asNumber(a), asNumber(b)));
}

// The interpreter: walk the tree on every call.
function interpret(node: Node, x: Value): Value {
  switch (node.type) {
    case "num": return int(node.value);
    case "arg": return x;
    case "bin": return arith(node.op, interpret(node.left, x), interpret(node.right, x));
  }
}

// Compiled code for int: closures over raw numbers, no tag checks inside.
function compileInt(node: Node): (n: number) => number {
  switch (node.type) {
    case "num": { const v = node.value; return () => v; }
    case "arg": return (n) => n;
    case "bin": {
      const f = OPS[node.op], l = compileInt(node.left), r = compileInt(node.right);
      return (n) => f(l(n), r(n));
    }
  }
}

// Compiled code for any argument: closures that still check tags.
function compileGeneric(node: Node): (a: Value) => Value {
  switch (node.type) {
    case "num": { const boxed = int(node.value); return () => boxed; }
    case "arg": return (a) => a;
    case "bin": {
      const op = node.op, l = compileGeneric(node.left), r = compileGeneric(node.right);
      return (a) => arith(op, l(a), r(a));
    }
  }
}

const THRESHOLD = 3;
type Code = (a: Value) => Value | null; // null means that the guard failed

class JitFunction {
  readonly name: string;
  readonly body: Node;
  calls = 0; // calls in the interpreter since the last compilation
  seen = new Set<Value["kind"]>(); // the profile: the kinds of the arguments
  code: Code | null = null;
  kind = "";
  counts = { interpreter: 0, compiled: 0, deoptimizations: 0 };

  constructor(name: string, body: Node) {
    this.name = name;
    this.body = body;
  }

  compile(): void {
    let how: string;
    if (this.seen.size === 1 && this.seen.has("int")) {
      const raw = compileInt(this.body);
      this.code = (a) => (a.kind === "int" ? int(raw(a.n)) : null); // the guard
      this.kind = "int";
      how = "specialized for int";
    } else {
      this.code = compileGeneric(this.body);
      this.kind = how = "generic";
    }
    console.log(`* compile ${this.name}: arguments seen: ${[...this.seen].sort().join(", ")}; ${how}`);
  }

  call(a: Value): [Value, string] {
    if (this.code === null && this.calls >= THRESHOLD) this.compile();
    if (this.code !== null) {
      const result = this.code(a);
      if (result !== null) {
        this.counts.compiled++;
        return [result, `compiled, ${this.kind}`];
      }
      console.log(`* deoptimize ${this.name}: guard failed, ${show(a)} is not an int`);
      this.counts.deoptimizations++;
      this.code = null;
      this.calls = 0;
    }
    this.calls++;
    this.counts.interpreter++;
    this.seen.add(a.kind);
    return [interpret(this.body, a), `interpreter, call ${this.calls}`];
  }
}

const f = new JitFunction("f", { type: "bin", op: "+", left: { type: "bin", op: "*", left: { type: "arg" }, right: { type: "arg" } }, right: { type: "num", value: 1 } });
for (const a of [int(1), int(2), int(3), int(4), float(2.5), int(6), int(7), int(8)]) {
  const [result, tier] = f.call(a);
  console.log(`f(${show(a)}) = ${show(result)} [${tier}]`);
}
console.log(Object.entries(f.counts).map(([k, v]) => `${k}: ${v}`).join(", "));
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

- Verdict: native. TypeScript compiles to JavaScript, and the JavaScript engine does the JIT compilation.
- `Value` is a discriminated union. After `a.kind === "int"`, the type checker narrows `a` and allows `a.n`. The guard in the compiled code is the same test at run time.
- The `switch` statements over `node.type` cover every member of `Node`, so the functions need no `default` case to satisfy `--strict`.
- The type annotations do not reach the engine. The engine collects its own type feedback while the program runs.
