```python run id=graph
edges = [("a", "b"), ("a", "c"), ("b", "c"), ("c", "d"), ("d", "a")]

graph = {}
for source, target in edges:
    graph.setdefault(source, []).append(target)
    graph.setdefault(target, [])

for node, neighbors in graph.items():
    print(node, "->", *neighbors)


def has_edge(x, y):
    return y in graph.get(x, [])


print("edge a->c:", "yes" if has_edge("a", "c") else "no")
print("edge c->a:", "yes" if has_edge("c", "a") else "no")
print("in-degree of c:", sum(neighbors.count("c") for neighbors in graph.values()))
print("edges:", sum(len(neighbors) for neighbors in graph.values()))
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

- Verdict: idiom. A `dict` from each node to a list of its neighbors is an adjacency list.
- `setdefault` adds a node with an empty list the first time the node appears.
