```csharp run id=ties
using System;
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
MidpointRounding[] modes =
{
    MidpointRounding.ToEven,
    MidpointRounding.AwayFromZero,
    MidpointRounding.ToZero,
    MidpointRounding.ToNegativeInfinity,
    MidpointRounding.ToPositiveInfinity,
};
foreach (var mode in modes)
{
    Console.WriteLine($"{mode,-19}{Math.Round(2.5, mode),4}{Math.Round(-2.5, mode),4}{Math.Round(2.6, mode),4}{Math.Round(-2.4, mode),4}");
}

Console.WriteLine($"Math.Round(2.5): {Math.Round(2.5)}, Math.Round(3.5): {Math.Round(3.5)}");
Console.WriteLine($"Math.Round(2.675, 2): {Math.Round(2.675, 2)}");
Console.WriteLine($"2.675 * 100 == 267.5: {2.675 * 100 == 267.5}");
Console.WriteLine($"Math.Round(2.675m, 2): {Math.Round(2.675m, 2)}");
Console.WriteLine($"Math.Round(2.665m, 2): {Math.Round(2.665m, 2)}");
Console.WriteLine($"(int)-2.7: {(int)-2.7}");
```

```output for=ties
ToEven                2  -2   3  -2
AwayFromZero          3  -3   3  -2
ToZero                2  -2   2  -2
ToNegativeInfinity    2  -3   2  -3
ToPositiveInfinity    3  -2   3  -2
Math.Round(2.5): 2, Math.Round(3.5): 4
Math.Round(2.675, 2): 2.68
2.675 * 100 == 267.5: True
Math.Round(2.675m, 2): 2.68
Math.Round(2.665m, 2): 2.66
(int)-2.7: -2
```

- Verdict: native. `Math.Round` uses half to even by default, and `MidpointRounding` selects the other rules.
- `ToZero`, `ToNegativeInfinity` and `ToPositiveInfinity` are directed: they apply to every value, not only to ties.
- The `decimal` value `2.675m` is exact, so it is a true tie and goes to the even digit 8. The `double` 2.675 is a little less than 2.675, so the correctly rounded result is 2.67, which Python's `round` gives. `Math.Round` gives 2.68 here. The product 2.675 * 100 rounds to exactly 267.5 in binary64, so a method that scales the value first sees a tie. Do not depend on the last digit of `Math.Round` on a `double`. Use `decimal` when the decimal digits matter.
