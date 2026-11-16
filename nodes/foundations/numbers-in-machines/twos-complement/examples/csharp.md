```csharp run id=negation
using System;

int x = 6;
Console.WriteLine($"-x == ~x + 1: {(-x == ~x + 1 ? "yes" : "no")}");
Console.WriteLine($"-1 in hex: {-1:x8}");
Console.WriteLine($"(sbyte)-6 widened to short: {(short)(sbyte)-6:x4}");

int min = int.MinValue;
Console.WriteLine($"unchecked(-min): {unchecked(-min)}");
try
{
    Console.WriteLine(checked(-min));
}
catch (OverflowException)
{
    Console.WriteLine("checked(-min): OverflowException");
}
```

```output for=negation
-x == ~x + 1: yes
-1 in hex: ffffffff
(sbyte)-6 widened to short: fffa
unchecked(-min): -2147483648
checked(-min): OverflowException
```

- Verdict: native. The signed types `sbyte`, `short`, `int` and `long` use two's complement.
- The format `x8` prints the bits of a negative value in hexadecimal. The widened `short` has copies of the sign bit in its high bits.
- In the default unchecked context, the negation of `int.MinValue` is the same value. In a `checked` context, it throws `OverflowException`.
