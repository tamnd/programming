```csharp run id=resequence
// Messages arrive with a key and a sequence number for each key, out of order and with one copy.
(string Key, int Seq)[] arrivals = [("A", 1), ("A", 3), ("B", 1), ("A", 2), ("B", 3), ("A", 3), ("B", 2), ("A", 4)];

var expected = new SortedDictionary<string, int>(StringComparer.Ordinal); // key -> next sequence number
var held = new Dictionary<string, Dictionary<int, string>>(); // key -> waiting messages
foreach (var (key, seq) in arrivals)
{
    int next = expected.GetValueOrDefault(key, 1);
    string name = $"{key}{seq}";
    if (!held.TryGetValue(key, out var h))
    {
        h = new Dictionary<int, string>();
        held[key] = h;
    }
    if (seq < next || h.ContainsKey(seq))
    {
        Console.WriteLine($"{name}: copy, dropped");
        continue;
    }
    h[seq] = name;
    var output = new List<string>();
    while (h.Remove(next, out var v))
    {
        output.Add(v);
        next++;
    }
    expected[key] = next;
    Console.WriteLine(output.Count > 0
        ? $"{name}: deliver {string.Join(" ", output)}"
        : $"{name}: hold, waiting for {key}{next}");
}
int left = held.Values.Sum(d => d.Count);
Console.WriteLine($"held at the end: {left}; next: {string.Join(", ", expected.Select(kv => $"{kv.Key}{kv.Value}"))}");
```

```output
A1: deliver A1
A3: hold, waiting for A2
B1: deliver B1
A2: deliver A2 A3
B3: hold, waiting for B2
A3: copy, dropped
B2: deliver B2 B3
A4: deliver A4
held at the end: 0; next: A5, B4
```

- Verdict: idiom. A `SortedDictionary` with `StringComparer.Ordinal` holds the next number for each key, so the keys print in a fixed order on each platform.
- `Dictionary.Remove(key, out value)` tests for the next message and takes it out in one call.
- `GetValueOrDefault` gives 1 as the next number for a key that has not arrived yet.
