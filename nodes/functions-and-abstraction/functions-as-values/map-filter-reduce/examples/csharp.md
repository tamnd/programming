```csharp run id=map-filter-reduce-csharp
using System;
using System.Linq;

class Program
{
    static void Main()
    {
        var squares = Enumerable.Range(1, 6)
            .Where(x => x % 2 == 0)
            .Select(x => x * x)
            .ToList();
        Console.WriteLine(string.Join(" ", squares));
        Console.WriteLine(squares.Aggregate(0, (acc, x) => acc + x));
    }
}
```

```output for=map-filter-reduce-csharp
4 16 36
56
```

- Verdict: stdlib. LINQ calls map `Select`, filter `Where` and reduce `Aggregate`.
