```csharp run id=export
var rows = new (string Name, int Count)[] { ("pen", 3), ("book", 1) };
foreach (Exporter exporter in new Exporter[] { new CsvExporter(), new TextExporter() })
{
    Console.WriteLine(exporter.Export(rows));
}

interface IWriter
{
    string Header();
    string Row(string name, int count);
}

sealed class CsvWriter : IWriter
{
    public string Header() => "name,count";
    public string Row(string name, int count) => $"{name},{count}";
}

sealed class PlainTextWriter : IWriter
{
    public string Header() => "Inventory";
    public string Row(string name, int count) => $"{name}: {count}";
}

abstract class Exporter
{
    public string Export(IEnumerable<(string Name, int Count)> rows)
    {
        IWriter writer = CreateWriter(); // the factory method
        var lines = new List<string> { writer.Header() };
        lines.AddRange(rows.Select(r => writer.Row(r.Name, r.Count)));
        return string.Join("\n", lines);
    }

    protected abstract IWriter CreateWriter();
}

sealed class CsvExporter : Exporter
{
    protected override IWriter CreateWriter() => new CsvWriter();
}

sealed class TextExporter : Exporter
{
    protected override IWriter CreateWriter() => new PlainTextWriter();
}
```

```output
name,count
pen,3
book,1
Inventory
pen: 3
book: 1
```

- Verdict: idiom. The classic form: an `abstract` class with a `protected abstract` factory method. Methods in C# are not virtual unless declared so, so `Export` cannot be overridden.
- The product is named `PlainTextWriter`, because the implicit usings of a .NET project import `System.IO`, which has a `TextWriter` class of its own.
- A `Func<IWriter>` parameter is the form with no creator subclass. The `Microsoft.Extensions.DependencyInjection` container can also build a writer by type.
