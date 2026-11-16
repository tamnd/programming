```csharp run id=function-composition-csharp
using System;

static class Fn
{
    public static Func<A, C> Compose<A, B, C>(this Func<B, C> f, Func<A, B> g)
    {
        return x => f(g(x));
    }
}

class Program
{
    static void Main()
    {
        Func<int, int> inc = x => x + 1;
        Func<int, int> twice = x => x * 2;

        Console.WriteLine(twice.Compose(inc)(5));
        Console.WriteLine(inc.Compose(twice)(5));
        Console.WriteLine(inc.Compose(twice.Compose(inc))(5));
    }
}
```

```output for=function-composition-csharp
12
11
13
```

- Verdict: idiom. C# has no compose operator. The extension method `Compose` takes two `Func` delegates and returns a lambda that calls both.
