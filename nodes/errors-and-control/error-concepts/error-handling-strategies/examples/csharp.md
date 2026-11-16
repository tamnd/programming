```csharp run id=strategy-csharp
using System;

static int Divide(int a, int b)
{
    if (b == 0)
    {
        throw new DivideByZeroException("division by zero");
    }
    return a / b;
}

foreach (var (a, b) in new[] { (10, 2), (10, 0) })
{
    try
    {
        Console.WriteLine($"{a} / {b} = {Divide(a, b)}");
    }
    catch (DivideByZeroException e)
    {
        Console.WriteLine($"{a} / {b}: error: {e.Message}");
    }
}
```

```output for=strategy-csharp
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: exceptions. For an error that occurs often, .NET also has the Try pattern: a method such as `int.TryParse` returns `false` and throws nothing, so the caller tests the result with `if`.
