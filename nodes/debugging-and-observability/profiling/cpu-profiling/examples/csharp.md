```csharp run id=counts
using System.Runtime.CompilerServices;

Console.WriteLine($"fib(20) = {Work.Fib(20)}");
Console.WriteLine($"total = {Work.Total(["apple", "bread", "apple"])}");
foreach (var (name, count) in Calls.Counts.OrderBy(kv => kv.Key, StringComparer.Ordinal))
{
    Console.WriteLine($"{name,-6} {count,6}");
}

static class Calls
{
    public static readonly Dictionary<string, int> Counts = new();

    // The compiler fills in the name of the calling method.
    public static void Count([CallerMemberName] string name = "")
    {
        Counts[name] = Counts.GetValueOrDefault(name) + 1;
    }
}

static class Work
{
    public static int Fib(int n)
    {
        Calls.Count();
        return n < 2 ? n : Fib(n - 1) + Fib(n - 2);
    }

    public static int Price(string item)
    {
        Calls.Count();
        return item == "bread" ? 250 : 30;
    }

    public static int Total(string[] cart)
    {
        Calls.Count();
        return cart.Sum(Price);
    }
}
```

```output for=counts
fib(20) = 6765
total = 310
Fib     21891
Price       3
Total       1
```

- Verdict: library. The base library has no profiler API. The `dotnet-trace` tool collects traces of a running process from outside the program. It uses EventPipe, a part of the .NET runtime.
- `[CallerMemberName]` makes the compiler pass the name of the calling method, so the counter needs no string in each function.
- A `Dictionary` has no fixed order, so the example sorts the names before it prints them.
