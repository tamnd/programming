```csharp run id=seeds
using System;
using System.Collections.Generic;

static class Program
{
    // Reads records of the form [tag][length][value].
    static List<(byte Tag, byte[] Value)> Parse(ReadOnlySpan<byte> data)
    {
        var records = new List<(byte, byte[])>();
        int i = 0;
        while (i < data.Length)
        {
            if (data.Length - i < 2)
            {
                throw new FormatException($"truncated header at offset {i}");
            }
            byte tag = data[i];
            int length = data[i + 1];
            if (data.Length - i - 2 < length)
            {
                throw new FormatException($"truncated value at offset {i}");
            }
            records.Add((tag, data.Slice(i + 2, length).ToArray()));
            i += 2 + length;
        }
        return records;
    }

    // The fuzz target: any exception other than FormatException is a bug.
    static string FuzzOne(byte[] data)
    {
        try
        {
            return $"records={Parse(data).Count}";
        }
        catch (FormatException e)
        {
            return $"rejected: {e.Message}";
        }
    }

    static void Main()
    {
        byte[][] seeds = [[], [1, 2, (byte)'h', (byte)'i'], [1, 2, (byte)'h', (byte)'i', 2, 0], [(byte)'z'], [(byte)'~', 0xe1]];
        foreach (byte[] seed in seeds)
        {
            string shown = seed.Length == 0 ? "(empty)" : BitConverter.ToString(seed).Replace("-", " ").ToLowerInvariant();
            Console.WriteLine($"{shown}: {FuzzOne(seed)}");
        }
        Console.WriteLine($"seed corpus: {seeds.Length} inputs passed");
    }
}
```

```output
(empty): records=0
01 02 68 69: records=1
01 02 68 69 02 00: records=2
7a: rejected: truncated header at offset 0
7e e1: rejected: truncated value at offset 0
seed corpus: 5 inputs passed
```

The original parser throws `IndexOutOfRangeException` on the input of one byte.

```csharp fails expect=exception:IndexOutOfRangeException id=no-length-check
using System;

static class Program
{
    // Counts the records of the form [tag][length][value].
    static int Parse(ReadOnlySpan<byte> data)
    {
        int count = 0;
        int i = 0;
        while (i < data.Length)
        {
            int length = data[i + 1]; // no check that the length byte exists
            count++;
            i += 2 + length;
        }
        return count;
    }

    static void Main()
    {
        byte[][] seeds = [[], [1, 2, (byte)'h', (byte)'i'], [1, 2, (byte)'h', (byte)'i', 2, 0], [(byte)'z'], [(byte)'~', 0xe1]];
        foreach (byte[] seed in seeds)
        {
            Console.WriteLine($"records={Parse(seed)}");
        }
    }
}
```

- Verdict: library. SharpFuzz runs .NET code under afl-fuzz.
- `ReadOnlySpan<byte>` checks each index, and `Slice` checks its range, so an error in the bounds is an exception and not a silent read.
- `byte` is unsigned in C#, so the length byte needs no conversion.
- `FormatException` is the documented rejection. Any other exception escapes from `FuzzOne`.
