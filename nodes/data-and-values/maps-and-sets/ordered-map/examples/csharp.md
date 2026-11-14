```csharp run id=sorteddictionary
using System;
using System.Collections.Generic;
using System.Linq;

// SortedDictionary is a binary search tree: inserts and removals take log time.
var d = new SortedDictionary<int, string> { [30] = "c", [10] = "a", [50] = "e", [20] = "b", [40] = "d" };
Console.WriteLine(string.Join(" ", d.Keys));
Console.WriteLine($"{d.First().Key} {d.Last().Key}");

// SortedList keeps two sorted arrays, so it can index by position.
var list = new SortedList<int, string>(d);
Console.WriteLine($"{list.Keys[0]} {list.Values[2]} {list.IndexOfKey(40)} {list.IndexOfKey(35)}");

// A floor search with the keys of a SortedList and a binary search.
var keys = list.Keys;
int i = BinarySearchFloor(keys, 35);
Console.WriteLine(i >= 0 ? keys[i] : -1);

// A range: SortedSet has GetViewBetween, and the maps need a filter.
Console.WriteLine(string.Join(" ", d.Where(p => p.Key >= 20 && p.Key < 45).Select(p => p.Value)));

// A comparer decides the order and which keys are the same.
var ci = new SortedDictionary<string, int>(StringComparer.OrdinalIgnoreCase) { ["apple"] = 1 };
ci["APPLE"] = 2;
Console.WriteLine($"{ci.Count} {ci["Apple"]}");

static int BinarySearchFloor(IList<int> ks, int x)
{
    int lo = 0, hi = ks.Count;
    while (lo < hi)
    {
        int mid = (lo + hi) / 2;
        if (ks[mid] <= x) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;
}
```

```output
10 20 30 40 50
10 50
10 c 3 -1
30
b c d
1 2
```

- Verdict: stdlib. SortedDictionary is a binary search tree, and SortedList keeps two sorted arrays, so it can index by position and IndexOfKey gives a position or -1. Neither has a floor method, so the example searches the keys of a SortedList. A comparer sets the order and which keys are the same.
