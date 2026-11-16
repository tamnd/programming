```javascript run id=graph
const edges = [["a", "b"], ["a", "c"], ["b", "c"], ["c", "d"], ["d", "a"]];

const graph = new Map();
for (const [source, target] of edges) {
  if (!graph.has(source)) graph.set(source, []);
  if (!graph.has(target)) graph.set(target, []);
  graph.get(source).push(target);
}

for (const [node, neighbors] of graph) {
  console.log(`${node} -> ${neighbors.join(" ")}`);
}

const hasEdge = (x, y) => (graph.get(x) ?? []).includes(y);
const yesNo = (value) => (value ? "yes" : "no");

console.log("edge a->c:", yesNo(hasEdge("a", "c")));
console.log("edge c->a:", yesNo(hasEdge("c", "a")));

let inDegree = 0;
let edgeCount = 0;
for (const neighbors of graph.values()) {
  inDegree += neighbors.filter((n) => n === "c").length;
  edgeCount += neighbors.length;
}
console.log("in-degree of c:", inDegree);
console.log("edges:", edgeCount);
```

```output for=graph
a -> b c
b -> c
c -> d
d -> a
edge a->c: yes
edge c->a: no
in-degree of c: 2
edges: 5
```

- Verdict: idiom. A `Map` from each node to an array of neighbors is an adjacency list.
- The in-degree needs a scan of every list, because the map stores only outgoing edges.
