```csharp run id=interlocked
int hits = 0;
Parallel.For(0, 4, _ =>
{
    for (int i = 0; i < 100_000; i++)
    {
        Interlocked.Increment(ref hits);
    }
});
Console.WriteLine(hits);

int old = Interlocked.CompareExchange(ref hits, 0, 400_000);
Console.WriteLine($"{old} {hits}");

long largest = 0;
Parallel.ForEach(new long[] { 7, 42, 13 }, value =>
{
    long seen;
    do
    {
        seen = Interlocked.Read(ref largest);
        if (value <= seen) return;
    } while (Interlocked.CompareExchange(ref largest, value, seen) != seen);
});
Console.WriteLine(largest);
```

```output for=interlocked
400000
400000 0
42
```

- Verdict: stdlib. `Interlocked` has atomic increment, add, exchange and compare-exchange. `Volatile.Read` and `Volatile.Write` give acquire and release order.
- The loop at the end is the usual pattern for an update that has no single atomic method: read the value, compute a new value, and retry if another thread changed it first.
