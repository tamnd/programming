```csharp run id=interlocked
using System;
using System.Threading;

int count = 0;
var workers = new Thread[2];
for (int t = 0; t < workers.Length; t++)
{
    workers[t] = new Thread(() =>
    {
        for (int i = 0; i < 100_000; i++)
        {
            Interlocked.Increment(ref count);
        }
    });
    workers[t].Start();
}
foreach (var worker in workers)
{
    worker.Join();
}
Console.WriteLine(count);
```

```output for=interlocked
200000
```

- Verdict: stdlib. `Interlocked.Increment` increments the variable and stores the result as one atomic operation.
- The C# specification makes reads and writes of `int` atomic, but it gives no guarantee for a read-modify-write such as `count++`.
- Reads and writes of `long`, `double` and `decimal` need not be atomic, so a racing read of these types can tear.
- `Interlocked.Increment` wraps on overflow and throws no exception.
