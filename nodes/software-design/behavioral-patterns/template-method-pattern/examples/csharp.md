```csharp run id=loader
Show("csv", new CsvLoader().Load(["# stock", "pen,3", "", "book,1"]));
Show("kv", new KeyValueLoader().Load(["pen=3", "# sold out", "cup=0"]));

static void Show(string label, List<Item> records) =>
    Console.WriteLine($"{label}: {string.Join(" ", records.Select(r => $"{r.Name}:{r.Count}"))}");

record Item(string Name, int Count);

abstract class Loader
{
    // Not virtual, so a derived class cannot override it.
    public List<Item> Load(IEnumerable<string> lines)
    {
        var records = new List<Item>();
        foreach (var raw in lines)
        {
            var line = raw.Trim();
            if (line.Length == 0 || line.StartsWith('#')) continue;
            var record = Parse(line);
            if (Accept(record)) records.Add(record);
        }
        return records;
    }

    protected abstract Item Parse(string line);

    protected virtual bool Accept(Item record) => true;
}

sealed class CsvLoader : Loader
{
    protected override Item Parse(string line)
    {
        var parts = line.Split(',');
        return new Item(parts[0], int.Parse(parts[1]));
    }
}

sealed class KeyValueLoader : Loader
{
    protected override Item Parse(string line)
    {
        var parts = line.Split('=');
        return new Item(parts[0], int.Parse(parts[1]));
    }

    protected override bool Accept(Item record) => record.Count > 0;
}
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: idiom. C# methods are non-virtual unless they are marked `virtual`, `abstract` or `override`. `Load` has none of these, so it is fixed. `Parse` is `abstract` and `Accept` is `virtual` with a default.
- A derived class that declares its own `Load` hides the base method and gets a compiler warning. A call through a `Loader` reference still runs the base `Load`.
- The defaults of the language match the pattern: a method must opt in to being a step that derived classes change.
