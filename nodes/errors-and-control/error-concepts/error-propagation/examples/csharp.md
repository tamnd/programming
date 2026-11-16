```csharp run id=propagate-csharp
using System;

foreach (string line in new[] { "port=8080", "port=x" })
{
    try
    {
        Console.WriteLine($"port {LoadConfig(line)}");
    }
    catch (ConfigException e)
    {
        Console.WriteLine($"error: {e.Message}: {e.InnerException!.Message}");
    }
}

static int ParsePort(string text)
{
    if (text.Length == 0) throw new FormatException("not a number: ");
    foreach (char c in text)
    {
        if (c < '0' || c > '9') throw new FormatException($"not a number: {text}");
    }
    return int.Parse(text);
}

static int LoadConfig(string line)
{
    string value = line.Substring(line.IndexOf('=') + 1);
    try
    {
        return ParsePort(value);
    }
    catch (FormatException e)
    {
        // Add context, and keep the original exception as the inner exception.
        throw new ConfigException($"config line '{line}'", e);
    }
}

class ConfigException : Exception
{
    public ConfigException(string message, Exception inner) : base(message, inner) { }
}
```

```output for=propagate-csharp
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. The runtime unwinds the stack until it finds a matching `catch` block. A new exception keeps the original one in `InnerException`. To pass the same exception on, a `catch` block uses `throw;`, which keeps the stack trace.
