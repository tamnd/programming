```csharp run id=threads-and-the-gil
using System;
using System.Threading;

// .NET has no global interpreter lock: these threads can run on several cores at once.
// So a shared counter must be changed with Interlocked, or under a lock.
long total = 0;
var threads = new Thread[4];
for (int t = 0; t < threads.Length; t++)
{
    threads[t] = new Thread(() =>
    {
        for (int i = 0; i < 100_000; i++)
        {
            Interlocked.Increment(ref total);
        }
    });
    threads[t].Start();
}
foreach (var t in threads)
{
    t.Join();
}
Console.WriteLine("4 threads added 100000 each with Interlocked.Increment");
Console.WriteLine($"total: {Interlocked.Read(ref total)}");
```

```output for=threads-and-the-gil
4 threads added 100000 each with Interlocked.Increment
total: 400000
```

- Verdict: absent. .NET has no GIL. A program can "create additional threads to execute code in parallel or concurrently with the primary thread".
- `Interlocked.Increment` adds 1 as one atomic step. `Interlocked.Read` returns "a 64-bit value, loaded as an atomic operation".
