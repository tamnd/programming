```csharp run id=promotion
using System;

static string YesNo(bool b) => b ? "yes" : "no";

int minusOne = -1;
uint one = 1;
Console.WriteLine($"-1 < 1u: {YesNo(minusOne < one)}");
Console.WriteLine($"type of -1 + 1u: {(minusOne + one).GetType().Name}");
Console.WriteLine($"unchecked((uint)-1): {unchecked((uint)minusOne)}");

byte b = 200;
Console.WriteLine($"unchecked((sbyte)200): {unchecked((sbyte)b)}");
try
{
    Console.WriteLine(checked((uint)minusOne));
}
catch (OverflowException)
{
    Console.WriteLine("checked((uint)-1): OverflowException");
}
```

```output for=promotion
-1 < 1u: yes
type of -1 + 1u: Int64
unchecked((uint)-1): 4294967295
unchecked((sbyte)200): -56
checked((uint)-1): OverflowException
```

- Verdict: native. C# has `byte`, `ushort`, `uint`, `ulong` and `nuint` next to the signed types. `byte` is unsigned and `sbyte` is signed.
- An `int` and a `uint` both become `long`, which holds all of their values, so `-1 < 1u` is true.
- A cast in an `unchecked` context keeps the bits. In a `checked` context, a value that does not fit throws `OverflowException`.
