```csharp run id=backtracking-resolver
// A backtracking dependency resolver: try the newest version first, and go back on a conflict.
var versions = new Dictionary<string, Dictionary<string, Req[]>>
{
    ["web"] = new()
    {
        ["2.0"] = [new("log", "2.0", "3.0", "")],
        ["1.1"] = [new("log", "1.1", "2.0", "")],
        ["1.0"] = [new("log", "1.0", "2.0", "")],
    },
    ["log"] = new() { ["2.0"] = [], ["1.2"] = [], ["1.1"] = [], ["1.0"] = [] },
};

int Cmp(string a, string b) =>
    a.Split('.').Select(int.Parse).Zip(b.Split('.').Select(int.Parse), (x, y) => x - y)
        .FirstOrDefault(d => d != 0);

bool Fits(string v, string lo, string hi) => Cmp(lo, v) <= 0 && Cmp(v, hi) < 0;

SortedDictionary<string, string>? Solve(List<Req> todo, SortedDictionary<string, string> chosen)
{
    if (todo.Count == 0)
    {
        return chosen;
    }
    var r = todo[0];
    var rest = todo.Skip(1).ToList();
    if (chosen.TryGetValue(r.Name, out var v))
    {
        if (Fits(v, r.Lo, r.Hi))
        {
            return Solve(rest, chosen);
        }
        Console.WriteLine($"  conflict: {r.Who} needs {r.Name} >={r.Lo},<{r.Hi}, but {r.Name} {v} is chosen");
        return null;
    }
    foreach (var version in versions[r.Name].Keys.Order(Comparer<string>.Create(Cmp)).Reverse())
    {
        if (!Fits(version, r.Lo, r.Hi))
        {
            continue;
        }
        Console.WriteLine($"try {r.Name} {version}");
        var next = rest.Concat(versions[r.Name][version].Select(d => d with { Who = $"{r.Name} {version}" })).ToList();
        var c = new SortedDictionary<string, string>(chosen, StringComparer.Ordinal) { [r.Name] = version };
        if (Solve(next, c) is { } result)
        {
            return result;
        }
    }
    return null;
}

List<Req>[] roots =
[
    [new("web", "1.0", "3.0", "app"), new("log", "1.0", "2.0", "app")],
    [new("web", "2.0", "3.0", "app"), new("log", "1.0", "2.0", "app")],
];
foreach (var root in roots)
{
    Console.WriteLine("app needs " + string.Join(", ", root.Select(r => $"{r.Name} >={r.Lo},<{r.Hi}")));
    var result = Solve(root, new SortedDictionary<string, string>(StringComparer.Ordinal));
    Console.WriteLine(result is null
        ? "result: no solution"
        : "result: " + string.Join(", ", result.Select(e => $"{e.Key} {e.Value}")));
}

// Who needs Name in the range [Lo, Hi).
record Req(string Name, string Lo, string Hi, string Who);
```

```output
app needs web >=1.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
try web 1.1
try log 1.2
result: log 1.2, web 1.1
app needs web >=2.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
result: no solution
```

- Verdict: idiom. A positional record holds one requirement, and `d with { Who = ... }` copies it with the name of the package that needs it.
- A `SortedDictionary` with `StringComparer.Ordinal` keeps the chosen versions sorted by name, and a new one is made for each try.
- The return type `SortedDictionary<string, string>?` is nullable, and `is { } result` tests for a solution.
