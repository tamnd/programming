```csharp run id=decimal-and-sizes
using System.Numerics;

// Integer division truncates, and % takes the sign of the dividend.
Console.WriteLine($"{7 / 2} {-7 / 2} {-7 % 2} {7 / 2.0}");

// decimal is a base-10 type, so 0.1 is exact.
Console.WriteLine($"{0.1 + 0.2} {0.1m + 0.2m} {0.1 + 0.2 == 0.3} {0.1m + 0.2m == 0.3m}");

// double division by zero gives infinity, but int and decimal division throw.
int zero = 0;
Console.WriteLine($"{double.IsPositiveInfinity(1.0 / zero)} {double.IsNaN(0.0 / zero)}");
try { Console.WriteLine(1m / zero); } catch (DivideByZeroException) { Console.WriteLine("DivideByZeroException"); }

// The sizes of the types are fixed, except nint, which has the size of a pointer.
Console.WriteLine($"{sizeof(int)} {sizeof(long)} {sizeof(decimal)} {sizeof(double)} {Int128.MaxValue}");

// BigInteger has no limit.
BigInteger f = 1;
for (int i = 2; i <= 30; i++) f *= i;
Console.WriteLine(f);

// A literal suffix chooses the type.
var values = new object[] { 1, 1L, 1u, 1.0, 1f, 1m };
Console.WriteLine(string.Join(" ", values.Select(v => v.GetType().Name)));
```

```output for=decimal-and-sizes
3 -3 -1 3.5
0.30000000000000004 0.3 False True
True True
DivideByZeroException
4 8 16 8 170141183460469231731687303715884105727
265252859812191058636308480000000
Int32 Int64 UInt32 Double Single Decimal
```

- Verdict: native. `/` truncates for integers. `decimal` is a base-10 type, so `0.1m + 0.2m == 0.3m` is true, and `0.1 + 0.2 == 0.3` is false for `double`. `double` division by zero gives infinity, but `decimal` division by zero throws `DivideByZeroException`. The sizes are fixed: `int` is 4 bytes, `long` is 8 and `decimal` is 16, and `Int128` and `BigInteger` hold larger values. A literal suffix chooses the type.
