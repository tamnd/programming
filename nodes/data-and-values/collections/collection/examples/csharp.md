```csharp run id=three-kinds
using System;
using System.Collections.Generic;

var words = new List<string> { "red", "blue", "red", "green" };
var unique = new SortedSet<string>(words);
var counts = new SortedDictionary<string, int>();
foreach (var w in words)
{
    counts[w] = counts.GetValueOrDefault(w) + 1;
}

static string Describe<T>(ICollection<T> c, T item) => $"{c.GetType().Name} {c.Count} {c.Contains(item)}";

Console.WriteLine(Describe(words, "red"));
Console.WriteLine(Describe(unique, "red"));
Console.WriteLine(Describe(counts.Keys, "red"));
Console.WriteLine($"{words[1]} {string.Join(",", unique)} {counts["red"]}");
```

```output for=three-kinds
List`1 4 True
SortedSet`1 3 True
KeyCollection 3 True
blue blue,green,red 2
```

- Verdict: stdlib. `System.Collections.Generic` has `List<T>`, `HashSet<T>`, `SortedSet<T>`, `Dictionary<TKey,TValue>` and `SortedDictionary<TKey,TValue>`.
- `Describe` takes any `ICollection<T>`. The keys of a dictionary are an `ICollection<TKey>` too.
- The name of a generic class has a backtick and the number of its type parameters, as in `` List`1 ``.
