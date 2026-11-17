```csharp run id=five-needs
using System;
using System.Collections.Generic;

var words = new List<string>("the cat saw the dog and the cat ran".Split(' '));

var seen = new HashSet<string>(words);
var counts = new Dictionary<string, int>();
foreach (var w in words) counts[w] = counts.GetValueOrDefault(w) + 1;
var line = new Queue<string>(words.GetRange(0, 3));
var first = line.Dequeue();
var second = line.Dequeue();
var sorted = new SortedDictionary<string, int>(counts, StringComparer.Ordinal);

Console.WriteLine($"list: {words.Count} words, third is {words[2]}");
Console.WriteLine($"set: {seen.Count} distinct");
Console.WriteLine($"map: the={counts["the"]} cat={counts["cat"]}");
Console.WriteLine($"queue: {first}, then {second}");
Console.WriteLine("sorted: " + string.Join(" ", sorted.Keys));
```

```output for=five-needs
list: 9 words, third is saw
set: 6 distinct
map: the=3 cat=2
queue: the, then cat
sorted: and cat dog ran saw the
```

- `List<T>`, `HashSet<T>`, `Dictionary<TKey,TValue>`, `Queue<T>` and `SortedDictionary<TKey,TValue>` are in `System.Collections.Generic`. `StringComparer.Ordinal` sorts the keys by their characters, not by the rules of a culture.
