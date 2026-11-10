```csharp run id=schema
using System;
using System.Collections.Generic;
using System.Linq;

(int[] Xs, int Want)[] weak = [([1, 2, 3], 2), ([1, 3, 2], 1), ([3, 1, 2], 0)];
(int[] Xs, int Want)[] strong = [.. weak, ([2, 2, 1], 0)];

bool Passes(Mutant m, (int[] Xs, int Want)[] suite) =>
    suite.All(c => Search.Argmax(c.Xs, m) == c.Want);

if (!Passes(Mutant.None, strong))
{
    throw new InvalidOperationException("the original must pass first");
}

Console.WriteLine($"{"mutant",-20}{"3 tests",-8}4 tests");
int weakKilled = 0, strongKilled = 0;
foreach (var m in Enum.GetValues<Mutant>().Skip(1))
{
    bool weakDead = !Passes(m, weak), strongDead = !Passes(m, strong);
    weakKilled += weakDead ? 1 : 0;
    strongKilled += strongDead ? 1 : 0;
    Console.WriteLine($"{Search.Names[m],-20}{(weakDead ? "killed" : "lived"),-8}{(strongDead ? "killed" : "lived")}");
}
Console.WriteLine($"{"killed",-20}{weakKilled} of 4  {strongKilled} of 4");

enum Mutant { None, GtToGe, GtToLt, LeftToBest, StartAt0 }

static class Search
{
    public static readonly Dictionary<Mutant, string> Names = new()
    {
        [Mutant.GtToGe] = "> to >=",
        [Mutant.GtToLt] = "> to <",
        [Mutant.LeftToBest] = "xs[i] to xs[best]",
        [Mutant.StartAt0] = "start at 0",
    };

    // Returns the index of the first largest item of a non-empty array.
    public static int Argmax(int[] xs, Mutant m)
    {
        int best = 0;
        for (int i = m == Mutant.StartAt0 ? 0 : 1; i < xs.Length; i++)
        {
            int left = m == Mutant.LeftToBest ? xs[best] : xs[i];
            bool bigger = m switch
            {
                Mutant.GtToGe => left >= xs[best],
                Mutant.GtToLt => left < xs[best],
                _ => left > xs[best],
            };
            if (bigger)
            {
                best = i;
            }
        }
        return best;
    }
}
```

```output
mutant              3 tests 4 tests
> to >=             lived   killed
> to <              killed  killed
xs[i] to xs[best]   killed  killed
start at 0          lived   lived
killed              2 of 4  3 of 4
```

- Verdict: library. .NET has no mutation tool in the SDK. Stryker.NET is the Stryker tool for C#.
- The `switch` expression on the enum is the mutant schema. The discard arm `_` keeps the original `>` for every other mutant.
- `[.. weak, ([2, 2, 1], 0)]` is a collection expression with a spread, so the strong suite is the weak suite plus the tie.
- Stryker reports a mutant that does not compile as a compile error and leaves it out of the mutation score. A run of the tests that ends in a timeout counts as detected.
