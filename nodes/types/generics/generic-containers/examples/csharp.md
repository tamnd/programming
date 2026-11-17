```csharp run id=typed-containers
using System;
using System.Collections.Generic;

var nums = new List<int> { 3, 1, 2 };
nums.Sort();
var groups = new Dictionary<string, List<int>>();
groups["odd"] = nums.FindAll(n => n % 2 == 1);
groups["even"] = nums.FindAll(n => n % 2 == 0);

foreach (var (key, list) in groups)
{
    Console.WriteLine($"{key}: {string.Join(",", list)}");
}
var seen = new HashSet<string> { "a", "b", "a" };
Console.WriteLine(seen.Count);
```

```output for=typed-containers
odd: 1,3
even: 2
2
```

- Verdict: stdlib. `List<T>`, `Dictionary<TKey,TValue>` and `HashSet<T>` are in `System.Collections.Generic`. A `List<int>` holds the `int` values themselves, with no boxing.
