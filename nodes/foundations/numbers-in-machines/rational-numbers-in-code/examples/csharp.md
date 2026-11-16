```csharp run id=ratio
using System;
using System.Numerics;

static Ratio Make(BigInteger n, BigInteger d)
{
    if (d.IsZero)
    {
        throw new DivideByZeroException();
    }
    if (d.Sign < 0)
    {
        n = -n;
        d = -d;
    }
    BigInteger g = BigInteger.GreatestCommonDivisor(n, d);
    return new Ratio(n / g, d / g);
}

static Ratio Add(Ratio a, Ratio b) => Make(a.N * b.D + b.N * a.D, a.D * b.D);

Console.WriteLine($"Make(6, -8): {Make(6, -8)}");
Ratio s = Add(Make(1, 10), Make(2, 10));
Console.WriteLine($"1/10 + 2/10: {s}");
Console.WriteLine($"equal to 3/10: {(s == Make(3, 10) ? "yes" : "no")}");
Console.WriteLine($"0.1 + 0.2 == 0.3: {(0.1 + 0.2 == 0.3 ? "yes" : "no")}");
Ratio h = Make(0, 1);
for (int k = 1; k <= 20; k++)
{
    h = Add(h, Make(1, k));
}
Console.WriteLine($"1/1 + ... + 1/20: {h}");
try
{
    Make(1, 0);
}
catch (DivideByZeroException)
{
    Console.WriteLine("Make(1, 0): DivideByZeroException");
}

record struct Ratio(BigInteger N, BigInteger D)
{
    public override string ToString() => $"{N}/{D}";
}
```

```output for=ratio
Make(6, -8): -3/4
1/10 + 2/10: 3/10
equal to 3/10: yes
0.1 + 0.2 == 0.3: no
1/1 + ... + 1/20: 55835135/15519504
Make(1, 0): DivideByZeroException
```

- Verdict: idiom. .NET has no rational type, so the block keeps a `record struct` of two `BigInteger` values in lowest terms.
- `BigInteger.GreatestCommonDivisor` gives the GCD, and the `record struct` gets `==` from its fields.
