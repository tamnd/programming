```csharp run id=truncated
using System;

static int FloorMod(int a, int n)
{
    int r = a % n;
    return r != 0 && (r < 0) != (n < 0) ? r + n : r;
}

Console.WriteLine($"-7 / 3: {-7 / 3}");
Console.WriteLine($"-7 % 3: {-7 % 3}");
var (q, r) = Math.DivRem(-7, 3);
Console.WriteLine($"Math.DivRem(-7, 3): ({q}, {r})");
Console.WriteLine($"FloorMod(-7, 3): {FloorMod(-7, 3)}");
Console.WriteLine($"5 % -4: {5 % -4}");
try
{
    int zero = 0;
    Console.WriteLine(1 / zero);
}
catch (DivideByZeroException e)
{
    Console.WriteLine($"1 / 0: {e.GetType().Name}");
}
```

```output for=truncated
-7 / 3: -2
-7 % 3: -1
Math.DivRem(-7, 3): (-2, -1)
FloorMod(-7, 3): 2
5 % -4: 1
1 / 0: DivideByZeroException
```

- Verdict: native. `/` truncates toward zero, and the remainder has the sign of the left operand. `Math.DivRem` returns both results as a tuple.
- Integer division by zero throws `DivideByZeroException`.
