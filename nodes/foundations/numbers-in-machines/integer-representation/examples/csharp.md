```csharp run id=checked-and-unchecked
using System.Buffers.Binary;

int max = int.MaxValue;
Console.WriteLine(unchecked(max + 1)); // wraps in an unchecked context
try
{
    Console.WriteLine(checked(max + 1));
}
catch (OverflowException e)
{
    Console.WriteLine($"checked: {e.GetType().Name}");
}

int v = 200, w = -5;
Console.WriteLine($"{(sbyte)v} {(byte)w} {sbyte.MinValue} {byte.MaxValue}"); // casts of variables wrap by default
Console.WriteLine($"{uint.MaxValue} {(uint)int.MaxValue + 1u} {Int128.MaxValue}");

// Write an int in each byte order.
var bytes = new byte[4];
BinaryPrimitives.WriteInt32BigEndian(bytes, -1000);
Console.WriteLine(Convert.ToHexString(bytes));
BinaryPrimitives.WriteInt32LittleEndian(bytes, -1000);
Console.WriteLine(Convert.ToHexString(bytes));
Console.WriteLine($"{-16 >> 2} {-16 >>> 28}");
```

```output for=checked-and-unchecked
-2147483648
checked: OverflowException
-56 251 -128 255
4294967295 2147483648 170141183460469231731687303715884105727
FFFFFC18
18FCFFFF
-4 15
```

- Verdict: native. C# has signed and unsigned types from 8 to 128 bits. Arithmetic is unchecked by default, so `int.MaxValue + 1` wraps, and in a `checked` expression the same addition throws `OverflowException`. A cast of a variable keeps the low bits. `BinaryPrimitives` writes an integer in a stated byte order, and `>>>` is the logical right shift.
