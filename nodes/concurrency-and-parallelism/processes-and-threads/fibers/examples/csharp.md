```csharp run id=iterator-fringe
using System;
using System.Collections.Generic;

static IEnumerable<int> Walk(object tree)
{
    if (tree is object[] children)
    {
        foreach (var child in children)
        {
            // Each level of the recursion is an iterator that passes values up.
            foreach (var leaf in Walk(child))
            {
                yield return leaf;
            }
        }
    }
    else
    {
        yield return (int)tree;
    }
}

static bool SameFringe(object x, object y)
{
    using var ex = Walk(x).GetEnumerator();
    using var ey = Walk(y).GetEnumerator();
    while (true)
    {
        bool hasX = ex.MoveNext();
        bool hasY = ey.MoveNext();
        if (hasX != hasY) return false;
        if (!hasX) return true;
        if (ex.Current != ey.Current) return false;
    }
}

object a = new object[] { new object[] { 1, 2 }, new object[] { 3, new object[] { 4 } } };
object b = new object[] { 1, new object[] { 2, new object[] { 3, 4 } } };
object c = new object[] { new object[] { 1, 2 }, new object[] { 3, 5 } };

foreach (var leaf in Walk(a))
{
    Console.WriteLine($"got {leaf}");
}
Console.WriteLine($"same fringe a b: {SameFringe(a, b).ToString().ToLower()}");
Console.WriteLine($"same fringe a c: {SameFringe(a, c).ToString().ToLower()}");
```

```output for=iterator-fringe
got 1
got 2
got 3
got 4
same fringe a b: true
same fringe a c: false
```

- Verdict: absent. C# has no fiber type. Iterators with `yield return` are the alternative, and they are stackless: an iterator keeps its position between requests and continues after the last `yield return`.
- A recursive iterator must loop over the inner iterator and give each value again with `yield return`.
- `SameFringe` moves two enumerators forward in turn with `MoveNext`, and the `using` declarations dispose of them when the method returns.
