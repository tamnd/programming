```csharp run id=binary-format-specifier
using System.Globalization;
using System.Numerics;

int n = 0b1010_1100; // binary literal with a digit separator
Console.WriteLine($"{n} {n:B} {n:X} {Convert.ToString(n, 8)}");
Console.WriteLine($"{5:B8} {Convert.ToString(-5, 2)}");

// Parse binary and hexadecimal text.
Console.WriteLine(int.Parse("101101", NumberStyles.AllowBinarySpecifier));
Console.WriteLine(Convert.ToInt32("ff", 16));

Console.WriteLine($"{BitOperations.PopCount((uint)n)} {BitOperations.LeadingZeroCount((uint)n)} {BitOperations.Log2(200u)}");
// BigInteger has no fixed width.
Console.WriteLine((BigInteger.One << 70).GetBitLength());
```

```output for=binary-format-specifier
172 10101100 AC 254
00000101 11111111111111111111111111111011
45
255
4 24 7
71
```

- Verdict: native. The format specifier `B` prints binary digits, and `B8` pads to 8 digits. `Convert.ToString` takes a base of 2, 8, 10 or 16 and shows a negative int as 32 bits. `NumberStyles.AllowBinarySpecifier` parses binary text. `BitOperations` counts bits and finds the highest set bit, and `BigInteger.GetBitLength` works at any size.
