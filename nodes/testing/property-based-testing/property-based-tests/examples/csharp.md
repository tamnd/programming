```csharp run id=checker
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;

// xorshift32 with Marsaglia's seed: the same numbers on every run.
sealed class Rng
{
    private uint state = 2463534242;

    public uint Next()
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    public long Between(long lo, long hi) => lo + (long)(Next() % (ulong)(hi - lo + 1));
}

static class Program
{
    // Runs a property on 100 generated cases and shrinks the first failure.
    static void Check<T>(string name, Func<Rng, T> generate, Func<T, bool> holds, Func<T, IEnumerable<T>> shrink)
    {
        var rng = new Rng();
        for (int trial = 1; trial <= 100; trial++)
        {
            T c = generate(rng);
            if (holds(c))
            {
                continue;
            }
            Console.WriteLine($"{name}: falsified on test {trial} by {c}");
            int steps = 0;
            // The list pattern matches when a smaller case still fails.
            while (shrink(c).Where(x => !holds(x)).Take(1).ToList() is [var smaller])
            {
                c = smaller;
                steps++;
            }
            Console.WriteLine($"{name}: shrunk in {steps} steps to {c}");
            return;
        }
        Console.WriteLine($"{name}: passed 100 tests");
    }

    // Gives lo, then x - d for d = (x - lo) / 2, / 4, ..., 1.
    static IEnumerable<int> ShrinkInt(int x, int lo)
    {
        if (x > lo)
        {
            yield return lo;
        }
        for (int d = (x - lo) / 2; d > 0; d /= 2)
        {
            yield return x - d;
        }
    }

    static int Pages(int items, int perPage) => items / perPage; // bug: drops the last page when it is not full

    static void Main()
    {
        Check(
            "round trip",
            rng => (int)rng.Between(int.MinValue, int.MaxValue),
            x => int.Parse(x.ToString(CultureInfo.InvariantCulture), CultureInfo.InvariantCulture) == x,
            x => Enumerable.Empty<int>());

        Check(
            "pages",
            rng => (Items: (int)rng.Between(0, 1000), PerPage: (int)rng.Between(1, 50)),
            c => Pages(c.Items, c.PerPage) * c.PerPage >= c.Items,
            c => ShrinkInt(c.Items, 0).Select(n => (Items: n, c.PerPage))
                .Concat(ShrinkInt(c.PerPage, 1).Select(p => (c.Items, PerPage: p))));
    }
}
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

- Verdict: library. FsCheck runs property-based tests from F#, C# and VB, and it shrinks a failing case to a minimal one.
- `uint` is a 32-bit unsigned type, so xorshift32 needs no masks. Arithmetic is unchecked by default and wraps.
- `Check<T>` takes delegates of the types `Func<Rng, T>`, `Func<T, bool>` and `Func<T, IEnumerable<T>>`. A named value tuple `(Items, PerPage)` prints as `(1, 2)`.
- `ShrinkInt` is an iterator with `yield return`, so the candidates are made one at a time and the loop stops at the first that fails.
- `int.Parse` and `ToString` use the culture of the thread unless they get `CultureInfo.InvariantCulture`. A round trip test with the default culture can pass on one machine and fail on another.
