```csharp run id=refutable-csharp
using System;

foreach (var line in new[] { "width=80", "oops", "height=24" })
{
    // A list pattern is refutable: it fails when the array has another length.
    if (line.Split('=', 2) is [var key, var value])
    {
        Console.WriteLine($"{key} -> {value}");
    }
    else
    {
        Console.WriteLine($"skip: {line}");
    }
}
```

```output for=refutable-csharp
width -> 80
skip: oops
height -> 24
```

- Verdict: native. A pattern after `is` can fail, and the `if` handles the failure. The list pattern `[var key, var value]` matches only an array of two elements. A deconstruction such as `var (a, b) = pair;` takes a tuple apart without a test.
