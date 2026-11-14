```csharp run id=hashset
using System;
using System.Collections.Generic;
using System.Linq;

// Add returns false when the set has the element already.
var s = new HashSet<string>();
Console.WriteLine($"{s.Add("a")} {s.Add("a")} {s.Count}");

// A comparer decides which elements are equal. This set ignores case.
var names = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "Ada", "ADA", "ada" };
Console.WriteLine($"{names.Count} {names.Contains("aDa")}");

// UnionWith, IntersectWith and ExceptWith change the set in place.
var a = new SortedSet<int> { 3, 1, 2 };
var b = new[] { 2, 3, 4 };
var union = new SortedSet<int>(a);
union.UnionWith(b);
var inter = new SortedSet<int>(a);
inter.IntersectWith(b);
var diff = new SortedSet<int>(a);
diff.ExceptWith(b);
Console.WriteLine($"{string.Join(",", union)} {string.Join(",", inter)} {string.Join(",", diff)}");

// SetEquals ignores order and duplicates. SequenceEqual does not.
Console.WriteLine($"{a.SetEquals(new[] { 2, 1, 3, 3 })} {a.SequenceEqual(new[] { 2, 1, 3 })}");

// A SortedSet can give the elements in a range.
Console.WriteLine(string.Join(",", union.GetViewBetween(2, 3)));
```

```output
True False 1
1 True
1,2,3,4 2,3 1
True False
2,3
```

- Verdict: stdlib. HashSet<T> and SortedSet<T>. Add returns false for an element that is there already. A comparer such as StringComparer.OrdinalIgnoreCase decides which elements are equal. UnionWith, IntersectWith and ExceptWith change the set in place. SetEquals ignores order and duplicates, and GetViewBetween gives a range of a SortedSet.
