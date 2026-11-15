```csharp run id=tiny-package-manager
// A tiny package manager: install with dependencies, refuse to break them, and remove unused ones.
var registry = new Dictionary<string, string[]>
{
    ["app"] = ["web", "log"],
    ["web"] = ["http", "log"],
    ["http"] = [],
    ["log"] = [],
    ["cli"] = ["log"],
};

// A name maps to true if the user asked for it, false if it came as a dependency.
var installed = new SortedDictionary<string, bool>(StringComparer.Ordinal);

string Label(bool asked) => asked ? "(asked)" : "(dependency)";

void Install(string name, bool asked)
{
    if (installed.TryGetValue(name, out bool was))
    {
        installed[name] = was || asked;
        return;
    }
    foreach (string dep in registry[name])
    {
        Install(dep, false);
    }
    installed[name] = asked;
    Console.WriteLine($"  install {name} {Label(asked)}");
}

List<string> NeededBy(string name) =>
    installed.Keys.Where(p => registry[p].Contains(name)).ToList();

void Remove(string name)
{
    var users = NeededBy(name);
    if (users.Count > 0)
    {
        Console.WriteLine($"  error: {name} is needed by {string.Join(", ", users)}");
        return;
    }
    installed.Remove(name);
    Console.WriteLine($"  remove {name}");
}

void Autoremove()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        foreach (string name in installed.Keys.ToList())
        {
            if (installed.TryGetValue(name, out bool asked) && !asked && NeededBy(name).Count == 0)
            {
                Remove(name);
                changed = true;
            }
        }
    }
}

(string Command, string Arg)[] steps =
[
    ("install", "app"), ("install", "cli"), ("remove", "log"),
    ("remove", "app"), ("autoremove", ""), ("list", ""),
];
foreach (var (command, arg) in steps)
{
    Console.WriteLine($"$ {command} {arg}".TrimEnd());
    switch (command)
    {
        case "install": Install(arg, true); break;
        case "remove": Remove(arg); break;
        case "autoremove": Autoremove(); break;
        default:
            foreach (var (name, asked) in installed)
            {
                Console.WriteLine($"  {name} {Label(asked)}");
            }
            break;
    }
}
```

```output
$ install app
  install http (dependency)
  install log (dependency)
  install web (dependency)
  install app (asked)
$ install cli
  install cli (asked)
$ remove log
  error: log is needed by app, cli, web
$ remove app
  remove app
$ autoremove
  remove web
  remove http
$ list
  cli (asked)
  log (dependency)
```

- Verdict: idiom. A `Dictionary` holds the registry, and a `SortedDictionary` with an ordinal comparer holds the installed packages in name order.
- `autoremove` loops over `installed.Keys.ToList()`, a copy, because `Remove` changes the dictionary during the loop.
- Local functions in the top-level program share the two dictionaries.
