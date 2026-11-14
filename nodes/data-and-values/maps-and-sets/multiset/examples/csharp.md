```csharp run id=countby
using System.Runtime.InteropServices;

string[] words = "the cat and the hat and the bat".Split(' ');

// CountBy (.NET 9) counts the items in one call.
var counted = words.CountBy(w => w).OrderByDescending(p => p.Value).ThenBy(p => p.Key, StringComparer.Ordinal);
Console.WriteLine(string.Join(" ", counted.Take(2)));

// A Dictionary of counts. GetValueRefOrAddDefault adds the key with 0 and
// returns a reference to the count, so one lookup does the update.
var counts = new Dictionary<string, int>();
foreach (var w in words) CollectionsMarshal.GetValueRefOrAddDefault(counts, w, out _)++;
Console.WriteLine($"{counts["the"]} {counts.GetValueOrDefault("dog")} {counts.Count} {counts.Values.Sum()}");

// Two multisets are equal when every key has the same count.
static Dictionary<char, int> Bag(string s) => s.GroupBy(ch => ch).ToDictionary(g => g.Key, g => g.Count());
static bool Same(Dictionary<char, int> a, Dictionary<char, int> b) =>
    a.Count == b.Count && a.All(p => b.TryGetValue(p.Key, out var n) && n == p.Value);
Console.WriteLine($"{Same(Bag("listen"), Bag("silent"))} {Same(Bag("aab"), Bag("ab"))}");
```

```output
[the, 3] [and, 2]
3 0 5 8
True False
```

- Verdict: idiom. .NET has no multiset type. CountBy from .NET 9 counts in one call, and CollectionsMarshal.GetValueRefOrAddDefault updates a Dictionary<TKey, int> with one lookup.
