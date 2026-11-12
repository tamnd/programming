```csharp run id=comparable-mixin
using System;

interface IOrdered<T> where T : IOrdered<T>
{
    int Compare(T other); // required

    bool LessThan(T other) => Compare(other) < 0; // provided
    bool Between(T lo, T hi) => lo.Compare((T)this) <= 0 && Compare(hi) <= 0;
}

record Version(int Major, int Minor) : IOrdered<Version>
{
    public int Compare(Version o) => Major != o.Major ? Major.CompareTo(o.Major) : Minor.CompareTo(o.Minor);
    public override string ToString() => $"{Major}.{Minor}";
}

class Program
{
    static string Yes(bool flag) => flag ? "yes" : "no";

    static void Main()
    {
        IOrdered<Version> v12 = new Version(1, 2);
        var v110 = new Version(1, 10);
        var v20 = new Version(2, 0);
        Console.WriteLine($"{v12} < {v110}: {Yes(v12.LessThan(v110))}");
        Console.WriteLine($"{v110} between {v12} and {v20}: {Yes(((IOrdered<Version>)v110).Between((Version)v12, v20))}");
        Console.WriteLine($"{v20} between {v12} and {v110}: {Yes(((IOrdered<Version>)v20).Between((Version)v12, v110))}");
    }
}
```

```output
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

- Verdict: idiom. Default interface methods are a mixin with no state. A default method is a member of the interface, not of the class, so the caller must use the interface type, as the casts show.
