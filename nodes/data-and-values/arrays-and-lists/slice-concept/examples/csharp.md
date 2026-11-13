```csharp run id=range-and-span
using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        // a[1..3] on an array makes a new array. The end index is not included.
        int[] a = { 10, 20, 30, 40, 50 };
        int[] part = a[1..3];
        part[0] = 99;
        Console.WriteLine($"{string.Join(",", part)} {string.Join(",", a)}");

        // ^1 counts from the end. a[^2..] is the last two items.
        Console.WriteLine($"{a[^1]} {string.Join(",", a[^2..])}");

        // A Span<T> slice is a view of the same memory.
        Span<int> view = a.AsSpan()[1..3];
        view[0] = 99;
        Console.WriteLine($"{view.Length} {string.Join(",", a)}");

        // GetRange copies a part of a List<T> into a new list.
        var list = new List<int>(a);
        List<int> range = list.GetRange(1, 2);
        range[0] = -1;
        Console.WriteLine($"{string.Join(",", range)} {list[1]}");

        // A range out of bounds throws.
        try
        {
            int[] bad = a[3..9];
        }
        catch (ArgumentOutOfRangeException e)
        {
            Console.WriteLine(e.GetType().Name);
        }
    }
}
```

```output
99,30 10,20,30,40,50
50 40,50
2 10,99,30,40,50
-1,30 99
ArgumentOutOfRangeException
```

- Verdict: native. a[1..3] on an array makes a new array, and ^ counts from the end. A Span<T> slice is a view, so a write through it changes the array. List<T>.GetRange copies. A range past the end throws ArgumentOutOfRangeException.
