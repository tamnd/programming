```csharp run id=forms
using System;

static int ToSignMagnitude(int n) => n < 0 ? 0x80 | -n : n;
static int ToOnes(int n) => n < 0 ? ~-n & 0xFF : n;
static int FromSignMagnitude(int b) => (b & 0x80) != 0 ? -(b & 0x7F) : b;
static int FromOnes(int b) => (b & 0x80) != 0 ? -(~b & 0xFF) : b;
static string Bin8(int b) => Convert.ToString(b, 2).PadLeft(8, '0');

Console.WriteLine($"-43 in sign-magnitude: {Bin8(ToSignMagnitude(-43))}");
Console.WriteLine($"-43 in ones' complement: {Bin8(ToOnes(-43))}");
Console.WriteLine($"10101011 in sign-magnitude: {FromSignMagnitude(0b10101011)}");
Console.WriteLine($"11010100 in ones' complement: {FromOnes(0b11010100)}");
Console.WriteLine($"10000000 in sign-magnitude: {FromSignMagnitude(0b10000000)}");
Console.WriteLine($"11111111 in ones' complement: {FromOnes(0b11111111)}");

double z = -0.0;
Console.WriteLine($"-0.0 == 0.0: {(z == 0.0 ? "yes" : "no")}");
Console.WriteLine($"double.IsNegative(-0.0): {(double.IsNegative(z) ? "yes" : "no")}");
Console.WriteLine($"1 / -0.0 is negative infinity: {(double.IsNegativeInfinity(1 / z) ? "yes" : "no")}");
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
-0.0 == 0.0: yes
double.IsNegative(-0.0): yes
1 / -0.0 is negative infinity: yes
```

- Verdict: idiom. The C# integral types are two's complement, so a program makes the other two forms by hand with `|`, `~` and `&`.
- `double.IsNegative` reads the sign bit, so it finds the sign of `-0.0`.
