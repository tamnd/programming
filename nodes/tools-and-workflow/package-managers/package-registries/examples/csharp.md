```csharp run id=tiny-registry
// A registry keeps each published version for ever. A yank hides a version from new resolutions only.
var pub = new Registry("public");
pub.Publish("log", "1.0.0");
pub.Publish("log", "1.1.0");
pub.Publish("log", "1.1.0");
pub.Yank("log", "1.1.0");
pub.Index("log");
Console.WriteLine($"install log: {pub.Find("log")}");
Console.WriteLine($"install log ==1.1.0: {pub.Find("log", "1.1.0")}");

var internalRegistry = new Registry("internal");
internalRegistry.Publish("billing", "1.0.0");
pub.Publish("billing", "99.0.0"); // someone takes the same name on the public registry
Registry[] registries = [internalRegistry, pub];
BestMatch(registries, "billing");
FirstIndex(registries, "billing");

static void BestMatch(Registry[] registries, string pkg)
{
    var best = registries
        .Select(r => (Version: r.Find(pkg), r.Name))
        .Where(p => p.Version is not null)
        .MaxBy(p => System.Version.Parse(p.Version!));
    Console.WriteLine($"best match over all indexes: {pkg} {best.Version} from {best.Name}");
}

static void FirstIndex(Registry[] registries, string pkg)
{
    foreach (var r in registries)
    {
        if (r.Find(pkg) is string v)
        {
            Console.WriteLine($"first index with the package: {pkg} {v} from {r.Name}");
            return;
        }
    }
}

class Registry(string name)
{
    public string Name { get; } = name;
    readonly Dictionary<string, Dictionary<string, bool>> packages = []; // package -> version -> yanked

    public void Publish(string pkg, string version)
    {
        if (!packages.TryGetValue(pkg, out var versions))
        {
            versions = [];
            packages[pkg] = versions;
        }
        if (!versions.TryAdd(version, false))
        {
            Console.WriteLine($"publish {Name}: {pkg} {version} error: the version exists");
            return;
        }
        Console.WriteLine($"publish {Name}: {pkg} {version} ok");
    }

    public void Yank(string pkg, string version)
    {
        packages[pkg][version] = true;
        Console.WriteLine($"yank {Name}: {pkg} {version}");
    }

    public void Index(string pkg)
    {
        var versions = packages.GetValueOrDefault(pkg) ?? [];
        var parts = versions.Keys.OrderBy(System.Version.Parse)
            .Select(v => versions[v] ? v + " (yanked)" : v);
        Console.WriteLine($"index of {pkg}: {string.Join(", ", parts)}");
    }

    // The newest version that is not yanked, or the pinned version: an exact pin can still get a yanked version.
    public string? Find(string pkg, string? pin = null)
    {
        var versions = packages.GetValueOrDefault(pkg) ?? [];
        if (pin is not null)
        {
            return versions.ContainsKey(pin) ? pin : null;
        }
        return versions.Where(e => !e.Value).Select(e => e.Key).MaxBy(System.Version.Parse);
    }
}
```

```output
publish public: log 1.0.0 ok
publish public: log 1.1.0 ok
publish public: log 1.1.0 error: the version exists
yank public: log 1.1.0
index of log: 1.0.0, 1.1.0 (yanked)
install log: 1.0.0
install log ==1.1.0: 1.1.0
publish internal: billing 1.0.0 ok
publish public: billing 99.0.0 ok
best match over all indexes: billing 99.0.0 from public
first index with the package: billing 1.0.0 from internal
```

- Verdict: idiom. `System.Version.Parse` reads a version such as 1.10.0, and LINQ `MaxBy` and `OrderBy` compare `Version` values, not strings.
- `TryAdd` returns false when the version exists, so one call both checks and publishes.
- `Find` has an optional parameter `pin`; a pinned version is returned even when it is yanked.
