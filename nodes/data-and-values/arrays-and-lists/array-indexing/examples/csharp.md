```csharp run id=index-from-end
using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        int[] a = { 10, 20, 30, 40, 50 };
        // Indices go from 0 to Length - 1, and ^n counts from the end.
        Console.WriteLine($"{a[0]} {a[a.Length - 1]} {a[^1]} {a[^2]}");

        // An Index is a value. GetOffset turns ^2 into a position for a given length.
        Index i = ^2;
        Console.WriteLine($"{i} {i.GetOffset(a.Length)} {i.IsFromEnd}");

        // A bad index on an array throws IndexOutOfRangeException. ^0 is one past the end.
        foreach (var bad in new[] { 5, -1 })
        {
            try { Console.WriteLine(a[bad]); }
            catch (IndexOutOfRangeException e) { Console.WriteLine(e.GetType().Name); }
        }
        try { Console.WriteLine(a[^0]); }
        catch (IndexOutOfRangeException) { Console.WriteLine("^0 is out of range"); }

        // A List<T> throws ArgumentOutOfRangeException instead.
        var list = new List<string> { "x", "y", "z" };
        try { Console.WriteLine(list[3]); }
        catch (ArgumentOutOfRangeException e) { Console.WriteLine($"{e.GetType().Name} {list[^1]}"); }
    }
}
```

```output
10 50 50 40
^2 3 True
IndexOutOfRangeException
IndexOutOfRangeException
^0 is out of range
ArgumentOutOfRangeException z
```

- Verdict: native. ^n counts from the end, and an Index is a value that GetOffset turns into a position. An array throws IndexOutOfRangeException for 5, for -1 and for ^0, which is one past the end. A List<T> throws ArgumentOutOfRangeException instead.
