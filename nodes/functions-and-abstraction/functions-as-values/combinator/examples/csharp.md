```csharp run id=combinator-csharp
using System;
using System.Linq;

class Program
{
    static T Identity<T>(T x) => x;

    static Func<B, A> Const<A, B>(A x) => _ => x;

    static Func<B, A, C> Flip<A, B, C>(Func<A, B, C> f) => (b, a) => f(a, b);

    static Func<T, bool> Both<T>(Func<T, bool> p, Func<T, bool> q) => x => p(x) && q(x);

    static void Main()
    {
        Console.WriteLine(Identity(5));
        Console.WriteLine(Const<int, int>(7)(99));
        Func<int, int, int> sub = (a, b) => a - b;
        Console.WriteLine(Flip(sub)(2, 10));
        var keep = Both<int>(x => x % 2 == 0, x => x > 0);
        Console.WriteLine(string.Join(" ", Enumerable.Range(-2, 7).Where(keep)));
    }
}
```

```output for=combinator-csharp
5
7
8
2 4
```

- Verdict: native. A generic method takes `Func` delegates and returns a lambda.
