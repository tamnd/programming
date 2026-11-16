```csharp run id=function-object-csharp
using System;
using System.Linq;

class Multiplier
{
    private readonly int factor;
    public int Calls;

    public Multiplier(int factor)
    {
        this.factor = factor;
    }

    public int Apply(int x)
    {
        Calls++;
        return x * factor;
    }
}

class Program
{
    static void Main()
    {
        var triple = new Multiplier(3);
        Func<int, int> f = triple.Apply;
        Console.WriteLine(f(5));
        Console.WriteLine(string.Join(" ", new[] { 1, 2, 3 }.Select(f)));
        Console.WriteLine(triple.Calls);
    }
}
```

```output for=function-object-csharp
15
3 6 9
4
```

- Verdict: idiom. A C# class cannot overload the call operator. The delegate `f` wraps the instance method `Apply`, so it holds the object `triple` and the method. Each call through `f` changes the field `Calls` of that object.
