```csharp run id=where-clause
using System;
using System.Collections.Generic;

Console.WriteLine(Bounds.Max(new[] { 3, 9, 4 }));
Console.WriteLine(Bounds.Max(new[] { "pear", "apple" }));
var list = Bounds.MakeAndFill<List<int>>(3);
Console.WriteLine(list.Count);

static class Bounds
{
    public static T Max<T>(IEnumerable<T> items) where T : IComparable<T>
    {
        var best = default(T);
        var first = true;
        foreach (var x in items)
        {
            if (first || x.CompareTo(best!) > 0)
            {
                best = x;
                first = false;
            }
        }
        return best!;
    }

    // Two constraints: an interface and a parameterless constructor.
    public static C MakeAndFill<C>(int n) where C : ICollection<int>, new()
    {
        var c = new C();
        for (var i = 0; i < n; i++)
        {
            c.Add(i);
        }
        return c;
    }
}
```

```output for=where-clause
9
pear
3
```

- Verdict: native. `where T : IComparable<T>` lets `Max` call `CompareTo`. `where C : ICollection<int>, new()` lets `MakeAndFill` create a `C` and call `Add`.
