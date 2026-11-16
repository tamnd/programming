```csharp run id=infinite
using System;
using System.Collections.Generic;
using System.Linq;

static IEnumerable<long> NaturalsFrom(long n)
{
    while (true)
    {
        yield return n;
        n++;
    }
}

static IEnumerable<T> Iterate<T>(Func<T, T> f, T x)
{
    while (true)
    {
        yield return x;
        x = f(x);
    }
}

static bool IsPrime(long n)
{
    for (long d = 2; d * d <= n; d++)
    {
        if (n % d == 0)
        {
            return false;
        }
    }
    return true;
}

Console.WriteLine(string.Join(" ", NaturalsFrom(2).Where(IsPrime).Take(10)));
Console.WriteLine(string.Join(" ", Iterate(x => 2 * x, 1L).Take(8)));
Console.WriteLine(NaturalsFrom(1).Select(n => n * n).First(s => s > 50));
```

```output for=infinite
2 3 5 7 11 13 17 19 23 29
1 2 4 8 16 32 64 128
64
```

- Verdict: stdlib. An iterator method with `yield return` inside `while (true)` is an infinite sequence. LINQ's `Where` and `Select` are deferred, and `Take` and `First` stop the enumeration.
- `Count`, `Sum`, `ToList` or `OrderBy` on `NaturalsFrom(2)` never return, and the compiler gives no warning.
