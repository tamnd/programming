```csharp run id=orderby-is-stable
using System;
using System.Collections.Generic;
using System.Linq;

var staff = new List<(string Name, string Team, int Grade)>
{
    ("ann", "ops", 3), ("bob", "dev", 2), ("cy", "ops", 2), ("di", "dev", 3), ("ed", "ops", 2),
};

static string Names(IEnumerable<(string Name, string Team, int Grade)> s) => string.Join(" ", s.Select(p => p.Name));

// LINQ OrderBy is stable: equal teams keep the name order.
Console.WriteLine(Names(staff.OrderBy(p => p.Team, StringComparer.Ordinal)));

// ThenBy adds a second key to the same stable sort.
Console.WriteLine(Names(staff.OrderBy(p => p.Team, StringComparer.Ordinal).ThenBy(p => p.Grade)));

// List<T>.Sort is not stable. The original position breaks the ties.
var indexed = staff.Select((p, i) => (p, i)).ToList();
indexed.Sort((a, b) =>
{
    int c = string.CompareOrdinal(a.p.Team, b.p.Team);
    return c != 0 ? c : a.i.CompareTo(b.i);
});
Console.WriteLine(Names(indexed.Select(x => x.p)));
```

```output
```

- Verdict: native. LINQ `OrderBy` is stable, so equal teams keep the name order, and `ThenBy` adds the grade. `List<T>.Sort` is not stable, so the program pairs each item with its position and compares the position when the teams are equal.
