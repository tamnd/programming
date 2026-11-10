```csharp run id=settings
Settings first = Settings.Instance;
Console.WriteLine($"main: {first.Level}");
Settings? seen = null;
var worker = new Thread(() =>
{
    Settings settings = Settings.Instance;
    Console.WriteLine("worker sets debug");
    settings.Level = "debug";
    seen = settings;
});
worker.Start();
worker.Join();
Console.WriteLine($"main: {Settings.Instance.Level}");
Console.WriteLine($"same instance: {ReferenceEquals(seen, first)}");

sealed class Settings
{
    private static readonly Lazy<Settings> instance = new(() => new Settings());

    public static Settings Instance => instance.Value;

    private volatile string level = "info";

    private Settings()
    {
        Console.WriteLine("creating the settings");
    }

    public string Level
    {
        get => level;
        set => level = value;
    }
}
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: True
```

- Verdict: stdlib. `Lazy<T>` creates the value on the first read of `Value`. Its default mode is thread-safe: only one thread runs the factory, and the others wait for it.
- A static field with an initializer, `static readonly Settings Instance = new();`, is the shorter form. The runtime runs static initialization at most once, but it is less precise about when.
- In an application that uses `Microsoft.Extensions.DependencyInjection`, register the class with `AddSingleton` instead, and take it as a constructor parameter.
