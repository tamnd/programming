```csharp run id=when-csharp
using System;

int[] items = { 10, 20, 30 };
foreach (int i in new[] { 1, 5 })
{
    try
    {
        Console.WriteLine($"items[{i}] = {items[i]}");
    }
    catch (IndexOutOfRangeException e)
    {
        Console.WriteLine($"items[{i}]: run-time error ({e.GetType().Name})");
    }
}
```

```output for=when-csharp
items[1] = 20
items[5]: run-time error (IndexOutOfRangeException)
```

- Verdict: native. The C# compiler finds syntax and type errors. The run-time system tests each array index and throws `IndexOutOfRangeException`.
