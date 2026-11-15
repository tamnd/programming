```csharp run id=tiny-build
// A tiny build system: rules, dependencies first, and a skip when a target is up to date.
var rules = new Dictionary<string, (string[] Deps, string Action)>
{
    ["app"] = (["main.o", "util.o"], "link"),
    ["main.o"] = (["main.c", "util.h"], "compile"),
    ["util.o"] = (["util.c", "util.h"], "compile"),
};
var store = new Dictionary<string, (string Value, int Time)>();
int clock = 0;

void Write(string key, string value) => store[key] = (value, ++clock);

void Build(string target, HashSet<string> done, List<string> ran)
{
    if (!done.Add(target))
    {
        return;
    }
    if (!rules.TryGetValue(target, out var rule))
    {
        if (!store.ContainsKey(target))
        {
            throw new KeyNotFoundException("no rule to make " + target);
        }
        return;
    }
    foreach (var dep in rule.Deps)
    {
        Build(dep, done, ran);
    }
    bool dirty = !store.TryGetValue(target, out var old)
        || rule.Deps.Any(dep => store[dep].Time > old.Time);
    if (dirty)
    {
        Write(target, rule.Action + "(" + string.Join(", ", rule.Deps) + ")");
        ran.Add(rule.Action + " " + target);
    }
}

void Run(string label)
{
    var ran = new List<string>();
    try
    {
        Build("app", [], ran);
    }
    catch (KeyNotFoundException e)
    {
        Console.WriteLine(label + ": error: " + e.Message);
        return;
    }
    Console.WriteLine(label + ": " + (ran.Count == 0 ? "nothing to do" : string.Join(", ", ran)));
}

foreach (var name in new[] { "main.c", "util.c", "util.h" })
{
    Write(name, "source of " + name);
}
Run("build 1");
Run("build 2");
Console.WriteLine("edit util.h");
Write("util.h", "edited");
Run("build 3");
Console.WriteLine("edit main.c");
Write("main.c", "edited");
Run("build 4");
Console.WriteLine("delete util.c");
store.Remove("util.c");
Run("build 5");
```

```output
build 1: compile main.o, compile util.o, link app
build 2: nothing to do
edit util.h
build 3: compile main.o, compile util.o, link app
edit main.c
build 4: compile main.o, link app
delete util.c
build 5: error: no rule to make util.c
```

- Verdict: idiom. A `Dictionary` of named tuples holds the rules, and a second `Dictionary` is the store.
- `Build` and `Write` are local functions of the top-level program, so they can use the store and the clock without a class.
- `TryGetValue` reads the rule and tells whether the target has one, in one call.
