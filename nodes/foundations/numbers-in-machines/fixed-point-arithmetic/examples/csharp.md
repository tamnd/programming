```csharp run id=scaled
using System;

const int Shift = 16;

static int Mul(int a, int b) => checked((int)(((long)a * b + (1L << (Shift - 1))) >> Shift));

int a = 3 << (Shift - 1);
int b = 9 << (Shift - 2);
Console.WriteLine($"1.5 * 2.25: {Mul(a, b) / 65536.0}");
int big = 300 << Shift;
try
{
    Console.WriteLine(Mul(big, big));
}
catch (OverflowException)
{
    Console.WriteLine("300 * 300: OverflowException");
}
long cents = 1999L * 3;
Console.WriteLine($"3 x 19.99: {cents / 100}.{cents % 100:D2}");
decimal tax = 59.97m * 0.0825m;
Console.WriteLine($"decimal tax: {tax}");
Console.WriteLine($"rounded: {Math.Round(tax, 2, MidpointRounding.AwayFromZero)}");
Console.WriteLine($"Math.Round(2.345m, 2): {Math.Round(2.345m, 2)}");
```

```output for=scaled
1.5 * 2.25: 3.375
300 * 300: OverflowException
3 x 19.99: 59.97
decimal tax: 4.947525
rounded: 4.95
Math.Round(2.345m, 2): 2.34
```

- Verdict: idiom. C# has no fixed-point type. The block keeps Q16.16 values in an `int`, multiplies in a `long`, and `checked` throws when the result does not fit.
- The `decimal` type keeps the scale of each value, so money is exact. `Math.Round` on a `decimal` rounds half to even by default, so 2.345 becomes 2.34.
