```csharp run id=decimal
using System;

double d = 0.1 + 0.2;
Console.WriteLine($"double  0.1 + 0.2 = {d:R}, == 0.3: {d == 0.3}");

decimal m = 0.1m + 0.2m;
Console.WriteLine($"decimal 0.1 + 0.2 = {m}, == 0.3: {m == 0.3m}");

Console.WriteLine($"decimal 1 / 3 = {1m / 3m}");
Console.WriteLine($"double  1 / 3 = {1.0 / 3.0:R}");

Console.WriteLine($"Math.BitIncrement(1.0) = {Math.BitIncrement(1.0):R}");
Console.WriteLine($"double.Epsilon = {double.Epsilon:R}");
Console.WriteLine($"Math.Round(2.5) = {Math.Round(2.5)}, Math.Round(3.5) = {Math.Round(3.5)}");
Console.WriteLine($"Math.Round(2.5, MidpointRounding.AwayFromZero) = {Math.Round(2.5, MidpointRounding.AwayFromZero)}");
```

```output
double  0.1 + 0.2 = 0.30000000000000004, == 0.3: False
decimal 0.1 + 0.2 = 0.3, == 0.3: True
decimal 1 / 3 = 0.3333333333333333333333333333
double  1 / 3 = 0.3333333333333333
Math.BitIncrement(1.0) = 1.0000000000000002
double.Epsilon = 5E-324
Math.Round(2.5) = 2, Math.Round(3.5) = 4
Math.Round(2.5, MidpointRounding.AwayFromZero) = 3
```

- Verdict: native. `double` is IEEE 754 binary64 and `float` is binary32. `decimal` is a 16-byte decimal type for money, with 28 to 29 significant digits. It is not an IEEE 754 format: it holds a 96-bit integer scaled by a power of 10, and it has no NaN and no infinity.
- `decimal` holds 0.1 exactly, but it still rounds: one third has no exact decimal form.
- `double.Epsilon` is the smallest positive subnormal double, not the gap after 1.0 that other languages call epsilon.
- `Math.Round` rounds a midpoint to the even neighbour by default, as IEEE 754 does. Pass `MidpointRounding.AwayFromZero` for the rule that most people learn at school.
