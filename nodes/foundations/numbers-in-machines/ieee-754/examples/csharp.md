```csharp run id=fields
using System;
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
foreach (double x in new[] { 1.0, -2.5, 0.1 })
{
    ushort b16 = BitConverter.HalfToUInt16Bits((Half)x);
    uint b32 = BitConverter.SingleToUInt32Bits((float)x);
    ulong b64 = BitConverter.DoubleToUInt64Bits(x);
    Console.WriteLine($"{x,4} binary16 {b16:x4} binary32 {b32:x8} binary64 {b64:x16}");
}

ulong b = BitConverter.DoubleToUInt64Bits(-2.5);
Console.WriteLine($"-2.5: sign {b >> 63}, exponent {(int)((b >> 52) & 0x7ff) - 1023}, fraction {b & ((1UL << 52) - 1):x}");

Console.WriteLine($"Half.MaxValue: {Half.MaxValue}, as double: {(double)Half.MaxValue}");
Console.WriteLine($"(Half)0.1 as double: {(double)(Half)0.1}");
Console.WriteLine($"(Half)65520.0: {(Half)65520.0}");
```

```output for=fields
   1 binary16 3c00 binary32 3f800000 binary64 3ff0000000000000
-2.5 binary16 c100 binary32 c0200000 binary64 c004000000000000
 0.1 binary16 2e66 binary32 3dcccccd binary64 3fb999999999999a
-2.5: sign 1, exponent 1, fraction 4000000000000
Half.MaxValue: 65500, as double: 65504
(Half)0.1 as double: 0.0999755859375
(Half)65520.0: Infinity
```

- Verdict: native. C# `float` and `double` are binary32 and binary64, and `System.Half` is binary16.
- `BitConverter.HalfToUInt16Bits`, `SingleToUInt32Bits` and `DoubleToUInt64Bits` give the bits as unsigned integers.
- `Half.MaxValue` prints as 65500, because `ToString` gives the shortest text that converts back to the same `Half`. As a `double`, the value is 65504.
- The program sets the invariant culture, so the text of infinity is the same on every machine.
