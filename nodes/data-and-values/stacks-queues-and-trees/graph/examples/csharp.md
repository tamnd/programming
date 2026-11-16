```csharp run id=graph
(string From, string To)[] edges = [("a", "b"), ("a", "c"), ("b", "c"), ("c", "d"), ("d", "a")];

var order = new List<string>();
var graph = new Dictionary<string, List<string>>();
foreach (var (from, to) in edges)
{
    foreach (string node in new[] { from, to })
    {
        if (!graph.ContainsKey(node))
        {
            graph[node] = new List<string>();
            order.Add(node);
        }
    }
    graph[from].Add(to);
}

foreach (string node in order)
{
    Console.WriteLine(node + " -> " + string.Join(" ", graph[node]));
}

bool HasEdge(string x, string y) => graph.TryGetValue(x, out var neighbors) && neighbors.Contains(y);

Console.WriteLine("edge a->c: " + (HasEdge("a", "c") ? "yes" : "no"));
Console.WriteLine("edge c->a: " + (HasEdge("c", "a") ? "yes" : "no"));
Console.WriteLine("in-degree of c: " + graph.Values.Sum(neighbors => neighbors.Count(n => n == "c")));
Console.WriteLine("edges: " + graph.Values.Sum(neighbors => neighbors.Count));
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

- Verdict: idiom. A `Dictionary<string, List<string>>` is an adjacency list. The program keeps a separate list of the nodes, so it prints them in the order they were added.
- `TryGetValue` gives `false` for a node that is not in the graph.
