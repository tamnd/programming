```csharp run id=equality-comparer-and-hash
using System;
using System.Collections.Generic;

var set = new HashSet<Sku> { new("A-1"), new("a-1") };
Console.WriteLine($"{set.Count} {set.Contains(new Sku("A-1"))}");

var cmp = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "A-1", "a-1" };
Console.WriteLine(cmp.Count);

// Equality and hash both ignore case, so equal values share a bucket.
sealed class Sku : IEquatable<Sku>
{
    public Sku(string code) => Code = code;

    public string Code { get; }

    public bool Equals(Sku? other) =>
        other is not null && StringComparer.OrdinalIgnoreCase.Equals(Code, other.Code);

    public override bool Equals(object? obj) => Equals(obj as Sku);

    public override int GetHashCode() => StringComparer.OrdinalIgnoreCase.GetHashCode(Code);
}
```

```output for=equality-comparer-and-hash
1 True
1
```

- Verdict: native. `Object` declares `Equals` and `GetHashCode`, and a class that overrides one of them must override the other, so that equal objects have the same hash code.
- `Sku` uses one comparer, `StringComparer.OrdinalIgnoreCase`, for both the equality and the hash, so they cannot disagree.
- A collection can also take an `IEqualityComparer<T>`, which gives a second pair of equality and hash without a change to the type.
