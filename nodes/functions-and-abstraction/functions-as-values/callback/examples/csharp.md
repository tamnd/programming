```csharp run id=callback-csharp
using System;
using System.Collections.Generic;

class Program
{
    static void Repeat(int n, Action<int> callback)
    {
        for (int i = 1; i <= n; i++)
        {
            callback(i);
        }
    }

    static void Tick(int i)
    {
        Console.WriteLine($"tick {i}");
    }

    static void Main()
    {
        var xs = new List<int> { 3, 1, 2 };
        xs.Sort((a, b) => b.CompareTo(a));
        Console.WriteLine(string.Join(" ", xs));
        Repeat(2, Tick);
    }
}
```

```output for=callback-csharp
3 2 1
tick 1
tick 2
```

- Verdict: native. `List<T>.Sort` takes a `Comparison<T>` delegate, and `Repeat` takes an `Action<int>` delegate. The method group `Tick` converts to the delegate.
