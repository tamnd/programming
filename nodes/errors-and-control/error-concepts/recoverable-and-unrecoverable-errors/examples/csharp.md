```csharp run id=recoverable-csharp
using System;

foreach (var text in new[] { "8080", "x" })
{
    var (port, note) = PortOrDefault(text, 80);
    Console.WriteLine($"{text} -> {port}{note}");
}

static (int, string) PortOrDefault(string text, int fallback)
{
    // A bad fallback is a bug in the caller: stop the process.
    if (fallback < 1 || fallback > 65535)
    {
        Environment.FailFast("fallback out of range");
    }
    // Bad text is an expected error: recover.
    return int.TryParse(text, out int n) ? (n, "") : (fallback, " (not a number)");
}
```

```output for=recoverable-csharp
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: stdlib. C# uses exceptions for most errors. For a state that the program cannot recover from, `Environment.FailFast` ends the process at once, and no `catch` or `finally` block runs. The expected error uses `int.TryParse`.
