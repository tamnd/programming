```csharp run id=dependency-tree
// Print a dependency tree, then find the packages that are shared (a diamond) and the packages with two versions.
var deps = new Dictionary<string, string[]>
{
    ["app"] = ["cli 1.0", "web 2.0"],
    ["cli 1.0"] = ["json 0.9", "log 1.2"],
    ["web 2.0"] = ["http 1.4", "json 1.1", "log 1.2"],
    ["http 1.4"] = ["log 1.2"],
    ["json 0.9"] = [],
    ["json 1.1"] = [],
    ["log 1.2"] = [],
};
var seen = new SortedSet<string>(StringComparer.Ordinal);

void Show(string package, string prefix)
{
    var children = deps[package];
    for (var i = 0; i < children.Length; i++)
    {
        var child = children[i];
        var last = i == children.Length - 1;
        var branch = last ? "`-- " : "+-- ";
        if (seen.Contains(child))
        {
            Console.WriteLine(prefix + branch + child + " (*)");
            continue;
        }
        Console.WriteLine(prefix + branch + child);
        seen.Add(child);
        Show(child, prefix + (last ? "    " : "|   "));
    }
}

Console.WriteLine("app");
Show("app", "");
Console.WriteLine("direct: " + string.Join(", ", deps["app"]));
Console.WriteLine("transitive: " + string.Join(", ", seen.Except(deps["app"])));

var users = deps
    .SelectMany(d => d.Value.Select(child => (Child: child, User: d.Key)))
    .GroupBy(x => x.Child)
    .OrderBy(g => g.Key, StringComparer.Ordinal);
Console.WriteLine("needed by more than one package:");
foreach (var g in users.Where(g => g.Count() > 1))
{
    Console.WriteLine($"  {g.Key}: {string.Join(", ", g.Select(x => x.User).Order(StringComparer.Ordinal))}");
}
Console.WriteLine("more than one version:");
foreach (var g in seen.GroupBy(p => p.Split(' ')[0]).Where(g => g.Count() > 1))
{
    Console.WriteLine($"  {g.Key}: {string.Join(", ", g.Select(p => p.Split(' ')[1]))}");
}
```

```output
app
+-- cli 1.0
|   +-- json 0.9
|   `-- log 1.2
`-- web 2.0
    +-- http 1.4
    |   `-- log 1.2 (*)
    +-- json 1.1
    `-- log 1.2 (*)
direct: cli 1.0, web 2.0
transitive: http 1.4, json 0.9, json 1.1, log 1.2
needed by more than one package:
  log 1.2: cli 1.0, http 1.4, web 2.0
more than one version:
  json: 0.9, 1.1
```

- Verdict: idiom. A dictionary of arrays holds the graph, and a `SortedSet` with `StringComparer.Ordinal` keeps the shown packages in order.
- `seen.Except(deps["app"])` gives the transitive dependencies.
- LINQ `SelectMany` and `GroupBy` turn the graph into a map from each package to its users.
