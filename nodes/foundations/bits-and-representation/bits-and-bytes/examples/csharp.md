```csharp run id=flags-enum-and-binary-primitives
using System;
using System.Buffers.Binary;
using System.Numerics;

var perm = Perm.Read | Perm.Write;
perm &= ~Perm.Write;
perm ^= Perm.Exec;
Console.WriteLine($"{perm} {perm.HasFlag(Perm.Exec)} {BitOperations.PopCount((uint)perm)}");

Console.WriteLine($"{-8 >> 1} {-8 >>> 28} {1 << 32} {1L << 32}");
Console.WriteLine($"{Convert.ToString(0b1011_0110, 2)} {0xB6:X} {BitOperations.LeadingZeroCount(1u)}");

var buf = new byte[4];
BinaryPrimitives.WriteUInt32LittleEndian(buf, 0x12345678);
Console.WriteLine($"{Convert.ToHexString(buf)} {BitConverter.IsLittleEndian}");
Console.WriteLine($"{BinaryPrimitives.ReadUInt32BigEndian(buf):X8}");

[Flags]
enum Perm { None = 0, Read = 1, Write = 1 << 1, Exec = 1 << 2 }
```

```output for=flags-enum-and-binary-primitives
Read, Exec True 2
-4 15 1 4294967296
10110110 B6 31
78563412 True
78563412
```

- Verdict: native. C# has `&`, `|`, `^`, `~`, `<<`, `>>` and the unsigned right shift `>>>`. As in Java, the shift count of an `int` uses its low 5 bits. An enum with the `[Flags]` attribute names a set of bits, prints as a list of names, and supports `HasFlag`. `BitOperations` counts and finds bits, and `BinaryPrimitives` reads and writes integers in a stated byte order.
- `BitConverter.IsLittleEndian` tells the byte order of the machine. Code that writes files or network messages uses `BinaryPrimitives` with an explicit order, so the result does not depend on it.
