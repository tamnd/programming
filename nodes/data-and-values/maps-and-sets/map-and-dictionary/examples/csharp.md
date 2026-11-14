```csharp run id=dictionary
// A Dictionary maps each key to one value. The indexer inserts or updates.
var d = new Dictionary<string, int> { ["x"] = 1, ["y"] = 2 };
d["x"] = 10;
d["z"] = 3;
Console.WriteLine($"{d["x"]} {d.Count} {d.ContainsKey("y")}");

// Reading a missing key with the indexer throws KeyNotFoundException.
try
{
    Console.WriteLine(d["w"]);
}
catch (KeyNotFoundException)
{
    Console.WriteLine("KeyNotFoundException");
}

// TryGetValue and GetValueOrDefault do not throw.
Console.WriteLine($"{d.TryGetValue("w", out var w)} {w} {d.GetValueOrDefault("w", -1)}");

// Add throws when the key is already there, and TryAdd returns false.
try
{
    d.Add("x", 5);
}
catch (ArgumentException)
{
    Console.WriteLine("ArgumentException");
}
Console.WriteLine($"{d.TryAdd("x", 5)} {d["x"]}");

// Remove returns whether a key was removed. Sort the pairs to print them.
Console.WriteLine($"{d.Remove("y")} {d.Remove("y")}");
Console.WriteLine(string.Join(", ", d.OrderBy(p => p.Key).Select(p => $"{p.Key}={p.Value}")));
```

```output
10 3 True
KeyNotFoundException
False 0 -1
ArgumentException
False 10
True False
x=10, z=3
```

- Verdict: stdlib. The indexer inserts or updates on a write and throws KeyNotFoundException on a read of a missing key. TryGetValue and GetValueOrDefault do not throw. Add throws for a key that is there, and TryAdd returns false.
