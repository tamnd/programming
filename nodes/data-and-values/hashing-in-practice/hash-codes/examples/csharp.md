```csharp run id=comparers-and-hashcode-combine
using System;
using System.Collections.Generic;
using System.Linq;

var points = new Dictionary<Point, string> { [new Point(1, 2)] = "a" };
Console.WriteLine($"record key: {points[new Point(1, 2)]}");

var ages = new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase) { ["Alan"] = 1912 };
Console.WriteLine($"ALAN with an ignore-case comparer: {ages["ALAN"]}");

var arrays = new HashSet<int[]> { new[] { 1, 2 } };
var byContents = new HashSet<int[]>(new SequenceComparer()) { new[] { 1, 2 } };
Console.WriteLine($"array by reference: {arrays.Contains(new[] { 1, 2 })}, by contents: {byContents.Contains(new[] { 1, 2 })}");

var a = new Account("ada", 1);
var b = new Account("ada", 7);
Console.WriteLine($"same id: {a.Equals(b)}, same hash: {a.GetHashCode() == b.GetHashCode()}");

readonly record struct Point(int X, int Y);

sealed class Account(string id, int visits)
{
    public string Id { get; } = id;
    public int Visits { get; } = visits;
    public override bool Equals(object? o) => o is Account a && a.Id == Id;
    public override int GetHashCode() => HashCode.Combine(Id);  // seeded per process
}

sealed class SequenceComparer : IEqualityComparer<int[]>
{
    public bool Equals(int[]? x, int[]? y) => x is not null && y is not null && x.SequenceEqual(y);
    public int GetHashCode(int[] xs)
    {
        var h = new HashCode();
        foreach (var x in xs) h.Add(x);
        return h.ToHashCode();
    }
}
```

```output for=comparers-and-hashcode-combine
record key: a
ALAN with an ignore-case comparer: 1912
array by reference: False, by contents: True
same id: True, same hash: True
```

- Verdict: native. Every object has `Equals` and `GetHashCode`, and records generate both. An `IEqualityComparer<T>` gives a collection its own equality and hash, such as `StringComparer.OrdinalIgnoreCase`, or comparison of arrays by contents in place of by reference. `HashCode.Combine` and `HashCode.Add` mix fields with a seed that is random for each process, and string hashes are randomized in the same way, so a hash code must not be stored or sent to another process.
