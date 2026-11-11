```csharp run id=port
var configs = new[]
{
    new Dictionary<string, string> { ["port"] = "8080" },
    new Dictionary<string, string> { ["port"] = "x" },
    new Dictionary<string, string>(),
};

foreach (var config in configs)
{
    var line = PortSetting(config) switch
    {
        Ok(var port) => $"port {port}",
        Err(_, true) => "use the default 80",
        Err(var message, _) => $"error: {message}",
        _ => "unknown",
    };
    Console.WriteLine(line);
}

static Result PortSetting(Dictionary<string, string> config)
{
    if (!config.TryGetValue("port", out var text))
    {
        return new Err("missing key \"port\"", true);
    }
    if (!int.TryParse(text, out var n) || n < 1 || n > 65535)
    {
        return new Err($"invalid port \"{text}\"", false);
    }
    return new Ok(n);
}

abstract record Result;

record Ok(int Value) : Result;

record Err(string Message, bool Missing) : Result;
```

```output
port 8080
error: invalid port "x"
use the default 80
```

- Verdict: idiom. C# reports errors with exceptions. The .NET libraries also use the Try pattern: `int.TryParse` and `Dictionary.TryGetValue` return `bool` and give the value through an `out` parameter.
- A record hierarchy gives a result type, and a `switch` expression with positional patterns takes it apart.
- The hierarchy is not closed, so the compiler cannot prove that the `switch` covers every case. The discard arm `_` is needed to avoid a warning.
