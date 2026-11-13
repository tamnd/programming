```csharp run id=comparison-and-comparer
using System;
using System.Collections.Generic;
using System.Linq;

var people = new List<(string Name, int Age)> { ("cy", 31), ("ann", 31), ("bob", 25) };

// A Comparison<T> delegate returns a negative number, zero or a positive number.
Comparison<int> ascending = (a, b) => a.CompareTo(b);
Console.WriteLine($"{ascending(25, 31)} {ascending(31, 31)} {ascending(31, 25)}");

// Chain two keys: use the second key when the first gives zero.
var byAgeThenName = people.ToList();
byAgeThenName.Sort((a, b) =>
{
    int c = a.Age.CompareTo(b.Age);
    return c != 0 ? c : string.CompareOrdinal(a.Name, b.Name);
});
Console.WriteLine(string.Join(",", byAgeThenName.Select(p => p.Name)));

// LINQ takes keys instead: OrderBy, then ThenBy.
Console.WriteLine(string.Join(",", people.OrderByDescending(p => p.Age).ThenBy(p => p.Name, StringComparer.Ordinal).Select(p => p.Name)));

// Comparer<T>.Create turns a lambda into an IComparer<T> for a sorted collection.
var reverse = Comparer<int>.Create((a, b) => b.CompareTo(a));
var set = new SortedSet<int>(new[] { 2, 3, 1 }, reverse);
Console.WriteLine(string.Join(",", set));

// Subtraction overflows for large values.
int x = int.MinValue, y = 1;
Console.WriteLine($"{unchecked(x - y)} {x.CompareTo(y)}");
```

```output
-1 0 1
bob,ann,cy
ann,cy,bob
3,2,1
2147483647 -1
```

- Verdict: native. A `Comparison<int>` delegate returns -1, 0 or 1. `List<T>.Sort` takes a comparison that uses the name when the ages are equal. LINQ `OrderByDescending` and `ThenBy` take keys instead. `Comparer<int>.Create` makes an `IComparer<int>` that reverses the order of a `SortedSet`. `int.MinValue - 1` wraps to `int.MaxValue` in an unchecked context, and `CompareTo` gives the correct sign.
