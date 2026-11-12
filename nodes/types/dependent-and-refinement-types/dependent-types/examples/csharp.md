```csharp run id=validated-structs
using System;
using System.Collections.Generic;

Console.WriteLine(Discount(80, Percent.Of(25)));
var xs = new NonEmpty<string>("a", ["b", "c"]);
Console.WriteLine($"first {xs.First}, count {xs.Count}");
try
{
    Percent.Of(120);
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine($"ArgumentOutOfRangeException: {e.ParamName} = {e.ActualValue}");
}
Console.WriteLine(default(Percent).Value); // the struct default skips the check

static double Discount(double price, Percent pct) => price - price * pct.Value / 100.0;

readonly record struct Percent
{
    public int Value { get; }
    private Percent(int value) => Value = value;
    public static Percent Of(int n) =>
        n is >= 0 and <= 100 ? new Percent(n)
            : throw new ArgumentOutOfRangeException(nameof(n), n, "not in 0..100");
}

sealed record NonEmpty<T>(T First, IReadOnlyList<T> Rest)
{
    public int Count => 1 + Rest.Count;
}
```

```output for=validated-structs
60
first a, count 3
ArgumentOutOfRangeException: n = 120
0
```

- Verdict: idiom. C# has no refinement types. A record struct with a private constructor and a factory method checks the predicate when the value is made. The last line shows a gap of this idiom for structs: `default(Percent)` makes a value without the factory, here 0, so the invariant must allow the default value, or the type must be a class.
