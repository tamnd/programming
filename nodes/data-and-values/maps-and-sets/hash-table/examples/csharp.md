```csharp run id=gethashcode
using System;
using System.Collections.Generic;

// A record struct gets Equals and GetHashCode from its fields.
var byValue = new Dictionary<Point, string> { [new Point(1, 2)] = "found" };
Console.WriteLine(byValue[new Point(1, 2)]);

// A class without overrides is a key by reference, so an equal-looking key misses.
var byRef = new Dictionary<RefPoint, string> { [new RefPoint(1, 2)] = "found" };
Console.WriteLine(byRef.ContainsKey(new RefPoint(1, 2)));

// A constant hash code is legal but puts every key in one bucket.
// Each insert then compares the new key with all keys that are already there.
var bad = new Dictionary<BadKey, int>();
for (int i = 0; i < 1000; i++) bad[new BadKey(i)] = i;
Console.WriteLine($"{bad[new BadKey(500)]} {BadKey.Calls}");

// EnsureCapacity sizes the table once, before many inserts.
var sized = new Dictionary<int, int>();
Console.WriteLine(sized.EnsureCapacity(1000) >= 1000);

// string.GetHashCode is randomized per process in .NET, so do not store it.
Console.WriteLine("key".GetHashCode() == "key".GetHashCode());

readonly record struct Point(int X, int Y);

sealed class RefPoint(int x, int y)
{
    public int X { get; } = x;
    public int Y { get; } = y;
}

sealed class BadKey(int n) : IEquatable<BadKey>
{
    public static long Calls;
    public int N { get; } = n;
    public bool Equals(BadKey? other)
    {
        Calls++;
        return other is not null && other.N == N;
    }
    public override bool Equals(object? obj) => Equals(obj as BadKey);
    public override int GetHashCode() => 0;
}
```

```output
found
False
500 500000
True
True
```

- Verdict: stdlib. Dictionary uses GetHashCode to find a bucket and Equals to find the key. A record struct defines both from its fields, and a class without overrides is a key by reference. A constant hash code makes 1000 inserts call Equals 499500 times. EnsureCapacity sizes the table once.
