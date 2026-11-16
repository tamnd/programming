```csharp run id=contexts
using System;

int x = int.MaxValue;
Console.WriteLine($"unchecked(x + 1): {unchecked(x + 1)}");
try
{
    int y = checked(x + 1);
    Console.WriteLine(y);
}
catch (OverflowException e)
{
    Console.WriteLine($"checked(x + 1): {e.GetType().Name}");
}
long wide = (long)x + 1;
Console.WriteLine($"saturated: {Math.Clamp(wide, int.MinValue, int.MaxValue)}");
```

```output for=contexts
unchecked(x + 1): -2147483648
checked(x + 1): OverflowException
saturated: 2147483647
```

- Verdict: native. The `checked` and `unchecked` keywords choose the rule for an expression or a block. The default context is unchecked unless the project sets `CheckForOverflowUnderflow`.
- `Math.Clamp` on a wider type saturates.
