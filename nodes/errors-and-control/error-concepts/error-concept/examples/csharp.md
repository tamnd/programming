```csharp run id=error-csharp
using System;

foreach (var text in new[] { "42", "x42" })
{
    try
    {
        int n = int.Parse(text);
        Console.WriteLine($"{text} -> {n}");
    }
    catch (FormatException e)
    {
        Console.WriteLine($"{text} -> error ({e.GetType().Name})");
    }
}
```

```output for=error-csharp
42 -> 42
x42 -> error (FormatException)
```

- Verdict: native. C# reports errors with exceptions, and has no checked exceptions. `int.Parse` throws `FormatException` for a bad string. The method `int.TryParse` reports the same error with a `bool` result instead, for code that expects bad input.
