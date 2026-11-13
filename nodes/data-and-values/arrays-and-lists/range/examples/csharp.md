```csharp run id=enumerable-range-and-system-range
using System;
using System.Linq;

class Program
{
    static void Main()
    {
        // Enumerable.Range takes a start and a count, not an end.
        Console.WriteLine($"{string.Join(",", Enumerable.Range(2, 5))} {Enumerable.Range(1, 100).Sum()}");

        // A System.Range a..b on an array includes a and excludes b, and ^n counts from the end.
        int[] a = { 10, 20, 30, 40, 50 };
        Console.WriteLine($"{string.Join(",", a[1..3])} {string.Join(",", a[^2..])} {string.Join(",", a[..^3])} {a[^1]}");

        // A Range is a value. GetOffsetAndLength turns it into numbers for a given length.
        Range r = 1..^1;
        var (offset, length) = r.GetOffsetAndLength(a.Length);
        Console.WriteLine($"{r} {offset} {length} {a[..].Length == a.Length}");

        // A negative count throws.
        try
        {
            Enumerable.Range(0, -1);
        }
        catch (ArgumentOutOfRangeException e)
        {
            Console.WriteLine(e.GetType().Name);
        }
    }
}
```

```output
2,3,4,5,6 5050
20,30 40,50 10,20 50
1..^1 1 3 True
ArgumentOutOfRangeException
```

- Verdict: native. Enumerable.Range takes a start and a count, so Range(2, 5) gives 2 to 6. On an array, a[1..3] includes 1 and excludes 3, and ^n counts from the end. A System.Range is a value, and GetOffsetAndLength turns 1..^1 into an offset and a length. A negative count throws ArgumentOutOfRangeException at once.
