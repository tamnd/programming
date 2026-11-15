```csharp run id=count
bool[] flags = { false, true };
Light[] lights = Enum.GetValues<Light>();

var pairs = new List<Pair>();
var choices = new List<Choice>();
foreach (bool f in flags)
{
    foreach (Light l in lights)
    {
        pairs.Add(new Pair(f, l));
    }
    choices.Add(new FlagCase(f));
}
foreach (Light l in lights)
{
    choices.Add(new LampCase(l));
}

Console.WriteLine("pairs: " + pairs.Count);
Console.WriteLine(string.Join(" ", pairs.Select(p => Yn(p.Flag) + "/" + Name(p.Light))));
Console.WriteLine("choices: " + choices.Count);
Console.WriteLine(string.Join(" ", choices.Select(Show)));

static string Yn(bool b) => b ? "yes" : "no";
static string Name(Light l) => l.ToString().ToLowerInvariant();
static string Show(Choice c) => c switch
{
    FlagCase f => "flag:" + Yn(f.Flag),
    LampCase l => "light:" + Name(l.Light),
    _ => throw new ArgumentException("unknown case"),
};

enum Light { Red, Amber, Green }

// Product: a flag AND a light.
record Pair(bool Flag, Light Light);

// Sum: a flag OR a light.
abstract record Choice;
sealed record FlagCase(bool Flag) : Choice;
sealed record LampCase(Light Light) : Choice;
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: idiom. A `record` is a product. An abstract record with sealed records as its cases stands for a sum.
- Other code can derive one more record from `Choice`, so the `switch` needs the discard arm `_`. Without it, the compiler gives a warning.
