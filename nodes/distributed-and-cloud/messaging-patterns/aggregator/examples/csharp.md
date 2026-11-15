```csharp run id=aggregate-parts
using System;
using System.Collections.Generic;

var incoming = new (string Cid, int Seq, int Size, string Item)[]
{
    ("A", 2, 3, "gadget"),
    ("B", 1, 2, "widget"),
    ("A", 1, 3, "widget"),
    ("B", 2, 2, "gizmo"),
    ("A", 2, 3, "gadget"), // a second copy, as at-least-once delivery can give
    ("A", 3, 3, "widget"),
    ("B", 1, 2, "widget"), // a copy that comes after B is complete
};

var groups = new Dictionary<string, SortedDictionary<int, string>>();
var closed = new HashSet<string>();

foreach (var (cid, seq, size, item) in incoming)
{
    if (closed.Contains(cid))
    {
        Console.WriteLine($"late: {cid} {seq}/{size} discarded, the group is closed");
        continue;
    }
    if (!groups.TryGetValue(cid, out var parts))
    {
        parts = new SortedDictionary<int, string>();
        groups[cid] = parts;
    }
    if (!parts.TryAdd(seq, item))
    {
        Console.WriteLine($"duplicate: {cid} {seq}/{size} ignored");
        continue;
    }
    if (parts.Count == size) // the completion condition: all parts are here
    {
        Console.WriteLine($"{cid} complete: {string.Join(", ", parts.Values)}");
        groups.Remove(cid);
        closed.Add(cid);
    }
}
Console.WriteLine($"still open: {groups.Count}");
```

```output
B complete: widget, gizmo
duplicate: A 2/3 ignored
A complete: widget, gadget, widget
late: B 1/2 discarded, the group is closed
still open: 0
```

- Verdict: idiom. A `Dictionary` from the correlation id to a `SortedDictionary` of the parts is the store, and `TryAdd` returns false for a second copy of a part.
- `string.Join` on the values of the `SortedDictionary` gives the items in the order of their sequence numbers.
- The `HashSet` of closed ids discards late copies; in a long-running service, it needs a limit, as the LRU cache of closed keys in Apache Camel has.
