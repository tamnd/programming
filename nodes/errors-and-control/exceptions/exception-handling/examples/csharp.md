```csharp run id=ports
int done = 0;
foreach (var text in new[] { "8080", "70000", "abc" })
{
    try
    {
        Console.WriteLine($"port {ParsePort(text)}");
    }
    catch (ConfigException e)
    {
        var cause = e.InnerException?.GetType().Name ?? "none";
        Console.WriteLine($"error: {e.Message} (cause: {cause})");
    }
    finally
    {
        done++;
    }
}
Console.WriteLine($"checked {done}");

static int ParsePort(string text)
{
    int port;
    try
    {
        port = int.Parse(text);
    }
    catch (FormatException e)
    {
        throw new ConfigException($"not a number: {text}", e);
    }
    if (port < 1 || port > 65535)
    {
        throw new ConfigException($"out of range: {port}");
    }
    return port;
}

class ConfigException(string message, Exception? inner = null) : Exception(message, inner);
```

```output
port 8080
error: out of range: 70000 (cause: none)
error: not a number: abc (cause: FormatException)
checked 3
```

- Verdict: native. `throw` throws an object of a class that derives from `System.Exception`, and `try` with `catch` and `finally` handles it.
- C# has no checked exceptions. A method does not declare what it throws.
- A `catch` clause can add a filter, `catch (IOException e) when (...)`. The filter runs before the stack unwinds, and when it is false the search goes on.
- Inside a `catch` block, `throw;` throws the exception again and keeps its stack trace. `throw e;` starts the stack trace again at that point.
- The second argument of the constructor sets `InnerException`. `using` disposes a resource on both paths.
