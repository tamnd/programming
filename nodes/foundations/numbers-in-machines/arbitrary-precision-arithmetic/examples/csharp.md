```csharp run id=biginteger
using System;
using System.Numerics;

BigInteger two = 2;
Console.WriteLine($"2^64: {BigInteger.Pow(two, 64)}");
Console.WriteLine($"2^100: {BigInteger.Pow(two, 100)}");
BigInteger f = 1;
for (int i = 1; i <= 30; i++)
{
    f *= i;
}
Console.WriteLine($"30!: {f}");
Console.WriteLine($"ulong.MaxValue^3: {BigInteger.Pow(ulong.MaxValue, 3)}");
BigInteger big = BigInteger.Pow(two, 64);
try
{
    long x = (long)big;
    Console.WriteLine(x);
}
catch (OverflowException)
{
    Console.WriteLine("(long)2^64: OverflowException");
}
BigInteger n = big;
n++;
Console.WriteLine($"big: {big}");
Console.WriteLine($"n: {n}");
Console.WriteLine($"-7 / 2: {BigInteger.Divide(-7, 2)}");
```

```output for=biginteger
2^64: 18446744073709551616
2^100: 1267650600228229401496703205376
30!: 265252859812191058636308480000000
ulong.MaxValue^3: 6277101735386680762814942322444851025767571854389858533375
(long)2^64: OverflowException
big: 18446744073709551616
n: 18446744073709551617
-7 / 2: -3
```

- Verdict: stdlib. `System.Numerics.BigInteger` has the usual operators, and an `int` or a `ulong` converts to it without a cast.
- The explicit conversion to `long` throws an `OverflowException` when the value does not fit.
- `n++` makes a new value for `n`, and `big` keeps its value, because `BigInteger` is immutable.
