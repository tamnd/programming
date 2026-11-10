```csharp run id=widgets
var kits = new (string Name, IWidgetFactory Kit)[] { ("plain", new PlainFactory()), ("round", new RoundFactory()) };
foreach (var (name, kit) in kits)
{
    Console.WriteLine($"{name}: {LoginForm(kit)}");
}

static string LoginForm(IWidgetFactory widgets) =>
    $"{widgets.Button("Save").Render()} {widgets.Checkbox("Remember me", true).Render()}";

interface IWidget
{
    string Render();
}

interface IWidgetFactory
{
    IWidget Button(string label);
    IWidget Checkbox(string label, bool @checked);
}

record PlainButton(string Label) : IWidget
{
    public string Render() => $"[ {Label} ]";
}

record PlainCheckbox(string Label, bool Checked) : IWidget
{
    public string Render() => $"[{(Checked ? "x" : " ")}] {Label}";
}

record RoundButton(string Label) : IWidget
{
    public string Render() => $"( {Label} )";
}

record RoundCheckbox(string Label, bool Checked) : IWidget
{
    public string Render() => $"({(Checked ? "*" : " ")}) {Label}";
}

sealed class PlainFactory : IWidgetFactory
{
    public IWidget Button(string label) => new PlainButton(label);
    public IWidget Checkbox(string label, bool @checked) => new PlainCheckbox(label, @checked);
}

sealed class RoundFactory : IWidgetFactory
{
    public IWidget Button(string label) => new RoundButton(label);
    public IWidget Checkbox(string label, bool @checked) => new RoundCheckbox(label, @checked);
}
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. The classic form with interfaces and records.
- `System.Data.Common.DbProviderFactory` in .NET is an abstract factory for the classes of one database provider: connections, commands, parameters and others. Its factory methods are virtual with a default, so a provider that does not support a product still compiles (see the node).
- A dependency injection container often holds the factory. The program registers one concrete factory at startup, and each class that needs widgets takes an `IWidgetFactory` in its constructor.
