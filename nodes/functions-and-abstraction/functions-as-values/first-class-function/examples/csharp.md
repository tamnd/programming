```csharp run id=first-class-function-csharp
using System;
using System.Collections.Generic;

class Program
{
    static int Double(int x)
    {
        return x * 2;
    }

    static int ApplyTwice(Func<int, int> g, int x)
    {
        return g(g(x));
    }

    static Func<int, int> Adder(int n)
    {
        return x => x + n;
    }

    static void Main()
    {
        Func<int, int> f = Double;
        Console.WriteLine(f(5));

        var ops = new Dictionary<string, Func<int, int>>
        {
            ["double"] = Double,
            ["square"] = x => x * x,
        };
        Console.WriteLine(ops["square"](4));

        Console.WriteLine(ApplyTwice(Double, 3));

        Func<int, int> add5 = Adder(5);
        Console.WriteLine(add5(10));
    }
}
```

```output for=first-class-function-csharp
10
16
12
15
```

- Verdict: native. The delegate type `Func<int, int>` holds a method group such as `Double` or a lambda. The lambda in `Adder` captures `n`.
