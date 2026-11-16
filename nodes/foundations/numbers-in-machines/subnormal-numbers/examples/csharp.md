```csharp run id=tiny
using System;

static string Yes(bool b) => b ? "yes" : "no";

double minNormal = BitConverter.Int64BitsToDouble(1L << 52);
Console.WriteLine($"smallest normal: {minNormal}");
Console.WriteLine($"double.Epsilon: {double.Epsilon}");
Console.WriteLine($"float.Epsilon: {float.Epsilon}");

Console.WriteLine($"IsNormal(minNormal): {Yes(double.IsNormal(minNormal))}");
Console.WriteLine($"IsSubnormal(minNormal / 2): {Yes(double.IsSubnormal(minNormal / 2))}");
Console.WriteLine($"IsSubnormal(Epsilon): {Yes(double.IsSubnormal(double.Epsilon))}");

double x = 1.25 * minNormal;
Console.WriteLine($"x - minNormal: {x - minNormal}");
Console.WriteLine($"Epsilon / 2: {double.Epsilon / 2}");
Console.WriteLine($"1.0 + Epsilon == 1.0: {Yes(1.0 + double.Epsilon == 1.0)}");
```

```output for=tiny
smallest normal: 2.2250738585072014E-308
double.Epsilon: 5E-324
float.Epsilon: 1E-45
IsNormal(minNormal): yes
IsSubnormal(minNormal / 2): yes
IsSubnormal(Epsilon): yes
x - minNormal: 5.562684646268003E-309
Epsilon / 2: 0
1.0 + Epsilon == 1.0: yes
```

- Verdict: native. C# `double` is IEEE 754, with `double.IsSubnormal` and `double.IsNormal`.
- `double.Epsilon` is the smallest subnormal, not the machine epsilon. Added to 1.0 it changes nothing.
- .NET has no named constant for the smallest normal double, so the code builds it from its bits.
