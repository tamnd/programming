```csharp run id=rebuild
// Two rebuilders on one graph: file modification times (as in Make) and content hashes.
using System.Security.Cryptography;
using System.Text;

var steps = new (string Label, Action<Builder> Edit)[]
{
    ("clean build", b =>
    {
        foreach (var name in new[] { "main.c", "util.c", "util.h" })
        {
            b.Write(name, Graph.Sources[name]);
        }
    }),
    ("touch util.h", b => b.Mtime["util.h"] = Graph.Tick()),
    ("comment in main.c", b => b.Write("main.c", Graph.Sources["main.c"] + " // entry point")),
    // A copy from a backup keeps its old time, older than util.o.
    ("old util.c, old time", b => b.Write("util.c", "int util(void) { return 2; }", 1)),
};
var byTime = new Builder(false);
var byHash = new Builder(true);
Console.WriteLine($"{"step",-24}{"mtime",-20}hash");
foreach (var (label, edit) in steps)
{
    edit(byTime);
    edit(byHash);
    Console.WriteLine($"{label,-24}{byTime.Build(),-20}{byHash.Build()}");
}
Console.WriteLine("mtime: app matches a clean build: " + (byTime.Content["app"] == Graph.FromScratch(byTime.Content)));
Console.WriteLine("hash: app matches a clean build: " + (byHash.Content["app"] == Graph.FromScratch(byHash.Content)));

static class Graph
{
    public static readonly Dictionary<string, string[]> Rules = new()
    {
        ["main.o"] = ["main.c", "util.h"],
        ["util.o"] = ["util.c", "util.h"],
        ["app"] = ["main.o", "util.o"],
    };
    public static readonly string[] Order = ["main.o", "util.o", "app"];
    public static readonly Dictionary<string, string> Sources = new()
    {
        ["main.c"] = "int main(void) { return util(); }",
        ["util.c"] = "int util(void) { return 1; }",
        ["util.h"] = "int util(void);",
    };
    static int clock = 0;

    public static int Tick() => ++clock;

    // The compiler drops comments, so an object file depends only on the code.
    public static string Action(string target, Dictionary<string, string> content) =>
        target + "(" + string.Join(" | ", Rules[target].Select(dep => content[dep].Split("//", 2)[0].TrimEnd())) + ")";

    public static string Digest(string text) => Convert.ToHexString(SHA256.HashData(Encoding.UTF8.GetBytes(text)));

    public static string FromScratch(Dictionary<string, string> content)
    {
        var values = Sources.Keys.ToDictionary(name => name, name => content[name]);
        foreach (var target in Order)
        {
            values[target] = Action(target, values);
        }
        return values["app"];
    }
}

class Builder(bool byHash)
{
    public readonly Dictionary<string, string> Content = [];
    public readonly Dictionary<string, int> Mtime = [];
    // A target maps to the hashes of its dependencies at the last build.
    readonly Dictionary<string, Dictionary<string, string>> traces = [];

    public void Write(string name, string text, int time = 0)
    {
        Content[name] = text;
        Mtime[name] = time == 0 ? Graph.Tick() : time;
    }

    bool Dirty(string target)
    {
        if (!Content.ContainsKey(target))
        {
            return true;
        }
        return Graph.Rules[target].Any(dep => byHash
            ? traces[target][dep] != Graph.Digest(Content[dep])
            : Mtime[dep] > Mtime[target]);
    }

    public string Build()
    {
        var ran = new List<string>();
        foreach (var target in Graph.Order)
        {
            if (Dirty(target))
            {
                Write(target, Graph.Action(target, Content));
                traces[target] = Graph.Rules[target].ToDictionary(dep => dep, dep => Graph.Digest(Content[dep]));
                ran.Add(target);
            }
        }
        return ran.Count == 0 ? "-" : string.Join(" ", ran);
    }
}
```

```output
step                    mtime               hash
clean build             main.o util.o app   main.o util.o app
touch util.h            main.o util.o app   -
comment in main.c       main.o app          main.o
old util.c, old time    -                   util.o app
mtime: app matches a clean build: False
hash: app matches a clean build: True
```

- Verdict: idiom. `SHA256.HashData` hashes the bytes, and `Convert.ToHexString` gives a string, so `!=` compares the content.
- `Builder` has a primary constructor, `class Builder(bool byHash)`, and `Dirty` uses `Any` with a conditional expression for the two rebuilders.
- The steps are an array of named tuples, `(string Label, Action<Builder> Edit)`, so the loop can deconstruct each one.
