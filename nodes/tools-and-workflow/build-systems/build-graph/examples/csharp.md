```csharp run id=graph
// A build graph: a topological order, the waves of a parallel build, and a cycle check.
var rules = new Dictionary<string, string[]>
{
    ["app"] = ["main.o", "parse.o", "lex.o"],
    ["main.o"] = ["main.c", "parse.h"],
    ["parse.o"] = ["parse.c", "parse.h", "lex.h"],
    ["lex.o"] = ["lex.c", "lex.h"],
    ["parse.c"] = ["grammar.y"],
    ["parse.h"] = ["grammar.y"],
};

SortedSet<string> Nodes(Dictionary<string, string[]> r) =>
    new(r.Keys.Concat(r.Values.SelectMany(deps => deps)), StringComparer.Ordinal);

string[] DepsOf(Dictionary<string, string[]> r, string n) => r.TryGetValue(n, out var d) ? d : [];

// Kahn's algorithm: a node is ready when all its dependencies are done.
List<string> TopologicalOrder(Dictionary<string, string[]> r)
{
    var waiting = new Dictionary<string, int>();
    var users = new Dictionary<string, List<string>>();
    var ready = new SortedSet<string>(StringComparer.Ordinal);
    foreach (var n in Nodes(r))
    {
        waiting[n] = DepsOf(r, n).Length;
        users[n] = [];
        if (waiting[n] == 0)
        {
            ready.Add(n);
        }
    }
    foreach (var (target, deps) in r)
    {
        foreach (var dep in deps)
        {
            users[dep].Add(target);
        }
    }
    var order = new List<string>();
    while (ready.Count > 0)
    {
        var n = ready.Min!;
        ready.Remove(n);
        order.Add(n);
        foreach (var user in users[n])
        {
            if (--waiting[user] == 0)
            {
                ready.Add(user);
            }
        }
    }
    return order;
}

List<List<string>> Waves(Dictionary<string, string[]> r)
{
    var level = new Dictionary<string, int>();
    var output = new SortedDictionary<int, List<string>>();
    foreach (var n in TopologicalOrder(r))
    {
        int k = DepsOf(r, n).Select(dep => level[dep] + 1).DefaultIfEmpty(0).Max();
        level[n] = k;
        if (k > 0)
        {
            if (!output.ContainsKey(k))
            {
                output[k] = [];
            }
            output[k].Add(n);
        }
    }
    return output.Values.Select(w => w.Order(StringComparer.Ordinal).ToList()).ToList();
}

List<string>? FindCycle(Dictionary<string, string[]> r)
{
    var state = new Dictionary<string, int>();
    var path = new List<string>();

    List<string>? Visit(string n)
    {
        state[n] = 1;
        path.Add(n);
        foreach (var dep in DepsOf(r, n))
        {
            if (state.GetValueOrDefault(dep) == 1)
            {
                return [.. path.Skip(path.IndexOf(dep)), dep];
            }
            if (!state.ContainsKey(dep) && Visit(dep) is { } found)
            {
                return found;
            }
        }
        path.RemoveAt(path.Count - 1);
        state[n] = 2;
        return null;
    }

    foreach (var n in Nodes(r))
    {
        if (!state.ContainsKey(n) && Visit(n) is { } found)
        {
            return found;
        }
    }
    return null;
}

Console.WriteLine("order: " + string.Join(" ", TopologicalOrder(rules)));
var waves = Waves(rules);
for (int i = 0; i < waves.Count; i++)
{
    Console.WriteLine($"wave {i + 1}: {string.Join(" ", waves[i])}");
}
var none = FindCycle(rules);
Console.WriteLine("cycle: " + (none is null ? "none" : string.Join(" -> ", none)));
var bad = new Dictionary<string, string[]>(rules) { ["lex.h"] = ["lex.o"] };
Console.WriteLine("after lex.h <- lex.o:");
var order = TopologicalOrder(bad);
Console.WriteLine($"order: {string.Join(" ", order)} ({order.Count} of {Nodes(bad).Count} nodes)");
Console.WriteLine("cycle: " + string.Join(" -> ", FindCycle(bad)!));
```

```output
order: grammar.y lex.c lex.h lex.o main.c parse.c parse.h main.o parse.o app
wave 1: lex.o parse.c parse.h
wave 2: main.o parse.o
wave 3: app
cycle: none
after lex.h <- lex.o:
order: grammar.y lex.c main.c parse.c parse.h main.o (6 of 10 nodes)
cycle: lex.h -> lex.o -> lex.h
```

- Verdict: idiom. A `Dictionary<string, string[]>` holds the rules, and a `SortedSet` with `StringComparer.Ordinal` holds the ready nodes, so `Min` gives the node with the smallest name.
- `Waves` computes each level with LINQ: `Select` gives the levels of the dependencies plus one, and `DefaultIfEmpty(0).Max()` gives 0 for an input.
- `Visit` is a local function inside `FindCycle`, and `is { } found` tests the result for null and names it in one step.
