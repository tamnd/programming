```csharp run id=multiton
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

var usd = Currency.Of("USD");
Console.WriteLine($"USD has {usd.Decimals} decimals");
Console.WriteLine($"JPY has {Currency.Of("JPY").Decimals} decimals");
Console.WriteLine($"same USD object: {ReferenceEquals(Currency.Of("USD"), Currency.Of("USD"))}");
Console.WriteLine($"instances: {Currency.Count}");
try
{
    Currency.Of("XYZ");
}
catch (ArgumentException e)
{
    Console.WriteLine($"{e.Message}: unknown currency");
}

using var start = new Barrier(8);
var tasks = Enumerable.Range(0, 8).Select(i => Task.Factory.StartNew(() =>
{
    start.SignalAndWait(); // all eight threads ask at the same time
    return Currency.Of("EUR");
}, TaskCreationOptions.LongRunning)).ToArray();
var seen = new HashSet<Currency>((await Task.WhenAll(tasks)), ReferenceEqualityComparer.Instance);
Console.WriteLine($"8 threads ask for EUR: {seen.Count} object");
Console.WriteLine($"instances: {Currency.Count}");

sealed class Currency
{
    static readonly Dictionary<string, int> Valid = new() { ["USD"] = 2, ["EUR"] = 2, ["JPY"] = 0 };
    // GetOrAdd can run the factory twice under a race, so the value is a Lazy:
    // only the Lazy that wins the race is ever asked for its value.
    static readonly ConcurrentDictionary<string, Lazy<Currency>> Instances = new();

    public string Code { get; }
    public int Decimals { get; }

    Currency(string code, int decimals) => (Code, Decimals) = (code, decimals);

    public static Currency Of(string code)
    {
        if (!Valid.TryGetValue(code, out var d)) throw new ArgumentException(code);
        return Instances.GetOrAdd(code, c => new Lazy<Currency>(() => new Currency(c, d))).Value;
    }

    public static int Count => Instances.Count;
}
```

```output
USD has 2 decimals
JPY has 0 decimals
same USD object: True
instances: 2
XYZ: unknown currency
8 threads ask for EUR: 1 object
instances: 3
```

- Verdict: stdlib. `ConcurrentDictionary.GetOrAdd` adds one value per key, but its factory may run more than once when threads race. A `Lazy<T>` as the value makes the creation run once.
- When an extra object is harmless and only one must be kept, `GetOrAdd` with a plain factory is enough: only one key and value pair is added to the dictionary.
