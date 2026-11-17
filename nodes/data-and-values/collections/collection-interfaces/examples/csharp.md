```csharp run id=to-the-interface
using System;
using System.Collections.Generic;

static int Total(IEnumerable<int> xs)
{
    int sum = 0;
    foreach (var x in xs) sum += x;
    return sum;
}

static string Describe(IReadOnlyCollection<int> c) => $"size {c.Count}, total {Total(c)}";

var prices = new Dictionary<string, int> { ["tea"] = 3, ["cake"] = 4 };
Console.WriteLine("list: " + Describe(new List<int> { 1, 2, 3 }));
Console.WriteLine("set: " + Describe(new HashSet<int> { 4, 5 }));
Console.WriteLine("map values: " + Describe(prices.Values));

IEnumerable<int> squares = Squares(4);
Console.WriteLine($"{Total(squares)} {squares is IReadOnlyCollection<int>}");

static IEnumerable<int> Squares(int n)
{
    for (int i = 0; i < n; i++) yield return i * i;
}
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7
14 False
```

- Verdict: stdlib. `System.Collections.Generic` has `IEnumerable<T>`, `ICollection<T>`, `IList<T>`, `IDictionary<TKey,TValue>` and the read-only forms `IReadOnlyCollection<T>`, `IReadOnlyList<T>` and `IReadOnlyDictionary<TKey,TValue>`.
- `List<T>`, `HashSet<T>` and the `Values` of a `Dictionary` all implement `IReadOnlyCollection<int>`. An iterator method gives only `IEnumerable<int>`.
