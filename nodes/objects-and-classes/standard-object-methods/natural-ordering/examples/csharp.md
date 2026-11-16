```csharp run id=icomparable-of-t
using System;
using System.Collections.Generic;

var vs = new List<AppVersion> { new(1, 10), new(1, 2), new(0, 9) };
vs.Sort();
Console.WriteLine(string.Join(" ", vs));
vs.Sort((a, b) => b.Minor.CompareTo(a.Minor));
Console.WriteLine(string.Join(" ", vs));
Console.WriteLine(new AppVersion(1, 2).CompareTo(null));

sealed record AppVersion(int Major, int Minor) : IComparable<AppVersion>
{
    // A null reference is less than any object.
    public int CompareTo(AppVersion? other)
    {
        if (other is null) return 1;
        int c = Major.CompareTo(other.Major);
        return c != 0 ? c : Minor.CompareTo(other.Minor);
    }

    public override string ToString() => $"{Major}.{Minor}";
}
```

```output for=icomparable-of-t
0.9 1.2 1.10
1.10 0.9 1.2
1
```

- Verdict: native. A type that implements `IComparable<T>` has a default order, and `List<T>.Sort`, `Array.Sort` and `SortedSet<T>` use it.
- `CompareTo` returns less than zero, zero or more than zero. By convention, any object is greater than `null`, so `CompareTo(null)` returns a positive number.
- A `Comparison<T>` delegate or an `IComparer<T>` gives another order, here the minor number from largest to smallest.
