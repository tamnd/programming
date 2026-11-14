```csharp run id=ordereddictionary
using System.Collections.Specialized;

// OrderedDictionary<TKey,TValue> (.NET 9) keeps insertion order and has an index.
var m = new OrderedDictionary<string, int>();
m.Add("pear", 4);
m.Add("apple", 7);
m["fig"] = 2;
Console.WriteLine(string.Join(" ", m.Keys));

// Setting an existing key keeps its place. Remove and add moves it to the end.
m["pear"] = 9;
Console.WriteLine(string.Join(" ", m));
m.Remove("pear");
m.Add("pear", 1);
Console.WriteLine(string.Join(" ", m.Keys));

// Index operations: the entry at a position, the position of a key, insert at a position.
Console.WriteLine($"{m.GetAt(0).Key} {m.IndexOf("fig")} {m.IndexOf("kiwi")}");
m.Insert(0, "kiwi", 5);
Console.WriteLine(string.Join(" ", m.Keys));

// Dictionary<TKey,TValue> does not promise an order. Two dictionaries with the
// same pairs in a different order are equal as sets of pairs.
var d1 = new Dictionary<string, int> { ["x"] = 1, ["y"] = 2 };
var d2 = new Dictionary<string, int> { ["y"] = 2, ["x"] = 1 };
Console.WriteLine(d1.Count == d2.Count && !d1.Except(d2).Any());

// The older non-generic OrderedDictionary stores object keys and values.
var old = new OrderedDictionary { { "b", 1 }, { "a", 2 } };
Console.WriteLine(string.Join(" ", old.Keys.Cast<string>()));
```

```output
pear apple fig
[pear, 9] [apple, 7] [fig, 2]
apple fig pear
apple 1 -1
kiwi apple fig pear
True
b a
```

- Verdict: stdlib. OrderedDictionary<TKey,TValue> from .NET 9 keeps insertion order and adds GetAt, IndexOf and Insert by position. Dictionary<TKey,TValue> does not promise an order. The older non-generic OrderedDictionary holds object keys and values.
