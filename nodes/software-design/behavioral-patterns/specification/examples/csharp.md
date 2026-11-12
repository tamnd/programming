```csharp run id=containers
using System;
using System.Collections.Generic;
using System.Linq;

record Container(string Name, bool Refrigerated, bool Clean);

class Spec<T>
{
    private readonly Func<T, bool> test;

    public Spec(Func<T, bool> test) => this.test = test;

    public bool IsSatisfiedBy(T candidate) => test(candidate);
    public Spec<T> And(Spec<T> other) => new Spec<T>(c => IsSatisfiedBy(c) && other.IsSatisfiedBy(c));
    public Spec<T> Or(Spec<T> other) => new Spec<T>(c => IsSatisfiedBy(c) || other.IsSatisfiedBy(c));
    public Spec<T> Not() => new Spec<T>(c => !IsSatisfiedBy(c));
}

class Program
{
    static readonly List<Container> Containers = new()
    {
        new Container("c1", true, true),
        new Container("c2", true, false),
        new Container("c3", false, true),
    };

    static string Select(Spec<Container> spec) =>
        string.Join(", ", Containers.Where(spec.IsSatisfiedBy).Select(c => c.Name));

    static void Main()
    {
        var refrigerated = new Spec<Container>(c => c.Refrigerated);
        var clean = new Spec<Container>(c => c.Clean);

        Console.WriteLine("meat: " + Select(refrigerated.And(clean)));
        Console.WriteLine("not refrigerated: " + Select(refrigerated.Not()));
        Console.WriteLine("refrigerated or clean: " + Select(refrigerated.Or(clean)));
    }
}
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: idiom. `Spec<T>` wraps a `Func<T, bool>`, and `And`, `Or` and `Not` return new specifications. The method group `spec.IsSatisfiedBy` is the filter for `Where`.
