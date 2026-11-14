```csharp run id=tolookup
// ToLookup builds a read-only multimap. A missing key gives an empty sequence.
string[] words = ["ant", "bee", "cat", "ape", "bat"];
ILookup<char, string> byFirst = words.ToLookup(w => w[0]);
Console.WriteLine(string.Join(",", byFirst['a']) + " " + byFirst['z'].Count() + " " + byFirst.Contains('z'));
Console.WriteLine(string.Join(" ", byFirst.Select(g => $"{g.Key}:{g.Count()}")));

// A Dictionary of Lists is the usual multimap that can change.
var m = new Dictionary<string, List<int>>();
void Add(string k, int v)
{
    if (!m.TryGetValue(k, out var list))
    {
        list = [];
        m[k] = list;
    }
    list.Add(v);
}
Add("yellow", 1);
Add("blue", 2);
Add("yellow", 3);
Console.WriteLine(string.Join(",", m["yellow"]) + " " + m.ContainsKey("red"));

// Remove one value, and the key when its list is empty.
m["blue"].Remove(2);
if (m["blue"].Count == 0) m.Remove("blue");
Console.WriteLine(string.Join(" ", m.Keys) + " " + m.Values.Sum(l => l.Count));
```

```output
ant,ape 0 False
a:2 b:2 c:1
1,3 False
yellow 2
```

- Verdict: stdlib. ToLookup builds an ILookup<TKey,TElement>, a multimap that cannot change, where a missing key gives an empty sequence. A Dictionary<TKey, List<TValue>> is the multimap that can change.
