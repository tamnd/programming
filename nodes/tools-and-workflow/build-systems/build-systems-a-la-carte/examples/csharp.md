```csharp run id=models
// Two build systems from the same parts: a scheduler and a rebuilder.
// The tasks are a small spreadsheet. B3 has a dynamic dependency.
using System;
using System.Collections.Generic;
using System.Linq;

static class Model
{
    public static readonly Dictionary<string, Func<Func<string, int>, int>> Tasks = new()
    {
        ["B1"] = f => f("A1") + f("A2"),
        ["B2"] = f => f("B1") * 2,
        ["B3"] = f => f("C1") == 1 ? f("B2") : f("A2"),
    };
    // A topological scheduler must know the dependencies before the build.
    // For B3 it must list both branches.
    public static readonly Dictionary<string, string[]> StaticDeps = new()
    {
        ["B1"] = new[] { "A1", "A2" },
        ["B2"] = new[] { "B1" },
        ["B3"] = new[] { "C1", "B2", "A2" },
    };
    public static readonly string[] Targets = { "B2", "B3" };
}

interface IBuildSystem
{
    Dictionary<string, int> Store { get; }
    void SetInput(string key, int value);
    List<string> Build();
}

// Topological scheduler with a dirty bit.
class MakeLike : IBuildSystem
{
    public Dictionary<string, int> Store { get; } = new();
    readonly HashSet<string> dirty = new();

    public void SetInput(string key, int value)
    {
        Store[key] = value;
        dirty.Add(key); // writing a file sets its time, even with the same content
    }

    void Visit(string k, HashSet<string> seen, List<string> order)
    {
        if (!Model.Tasks.ContainsKey(k) || !seen.Add(k)) return;
        foreach (var d in Model.StaticDeps[k]) Visit(d, seen, order);
        order.Add(k);
    }

    public List<string> Build()
    {
        var order = new List<string>();
        var seen = new HashSet<string>();
        foreach (var t in Model.Targets) Visit(t, seen, order);
        var ran = new List<string>();
        foreach (var k in order)
        {
            if (!Store.ContainsKey(k) || Model.StaticDeps[k].Any(dirty.Contains))
            {
                Store[k] = Model.Tasks[k](d => Store[d]);
                dirty.Add(k);
                ran.Add(k);
            }
        }
        dirty.Clear();
        return ran;
    }
}

// Suspending scheduler with verifying traces.
class ShakeLike : IBuildSystem
{
    public Dictionary<string, int> Store { get; } = new();
    readonly Dictionary<string, (List<(string Key, string Hash)> Deps, string Result)> traces = new();
    HashSet<string> done = new();
    List<string> ran = new();

    public void SetInput(string key, int value) => Store[key] = value;

    int Fetch(string k)
    {
        if (!Model.Tasks.ContainsKey(k) || done.Contains(k)) return Store[k];
        if (traces.TryGetValue(k, out var t) && Store[k].ToString() == t.Result
            && t.Deps.All(d => Fetch(d.Key).ToString() == d.Hash))
        {
            done.Add(k);
            return Store[k];
        }
        var deps = new List<(string, string)>();
        int v = Model.Tasks[k](d =>
        {
            // The task waits here until d is up to date: this is the suspending scheduler.
            int dv = Fetch(d);
            deps.Add((d, dv.ToString()));
            return dv;
        });
        Store[k] = v;
        traces[k] = (deps, v.ToString());
        done.Add(k);
        ran.Add(k);
        return v;
    }

    public List<string> Build()
    {
        done = new();
        ran = new();
        foreach (var t in Model.Targets) Fetch(t);
        return ran;
    }
}

static class Program
{
    static string Show(List<string> ran) => ran.Count == 0 ? "-" : string.Join(" ", ran);

    static void Main()
    {
        IBuildSystem make = new MakeLike(), shake = new ShakeLike();
        foreach (var b in new[] { make, shake })
        {
            b.SetInput("A1", 10); b.SetInput("A2", 20); b.SetInput("C1", 1);
        }
        var steps = new (string Name, (string, int)[] Changes)[]
        {
            ("clean build", new (string, int)[0]),
            ("A2 = 20 again", new[] { ("A2", 20) }),
            ("A1 = 11, A2 = 19", new[] { ("A1", 11), ("A2", 19) }),
            ("C1 = 2", new[] { ("C1", 2) }),
            ("A1 = 12", new[] { ("A1", 12) }),
        };
        Console.WriteLine($"{"step",-18}{"make-like",-14}shake-like");
        foreach (var (name, changes) in steps)
        {
            foreach (var b in new[] { make, shake })
                foreach (var (k, v) in changes) b.SetInput(k, v);
            Console.WriteLine($"{name,-18}{Show(make.Build()),-14}{Show(shake.Build())}");
        }
        bool same = Model.Tasks.Keys.All(k => make.Store[k] == shake.Store[k]);
        Console.WriteLine($"same values: {same}");
        var s = shake.Store;
        Console.WriteLine($"B1, B2, B3 = {s["B1"]} {s["B2"]} {s["B3"]}");
    }
}
```

```output
step              make-like     shake-like
clean build       B1 B2 B3      B1 B2 B3
A2 = 20 again     B1 B2 B3      -
A1 = 11, A2 = 19  B1 B2 B3      B1
C1 = 2            B3            B3
A1 = 12           B1 B2 B3      B1 B2
same values: True
B1, B2, B3 = 31 62 19
```

- Verdict: idiom. A `Dictionary<string, Func<Func<string, int>, int>>` holds the tasks: each task is a function from the `fetch` callback to a value.
- A trace is a tuple of a list of `(Key, Hash)` tuples and the hash of the result. `All` stops at the first dependency whose hash changed.
- Both build systems implement `IBuildSystem`, so `Main` changes the inputs of both in one loop and compares their stores at the end.
