```csharp run id=async-limit
using System;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

const int Tasks = 1_000;
var permits = new SemaphoreSlim(10);
int active = 0;
int peak = 0;

async Task<int> Work(int n)
{
    await permits.WaitAsync();
    try
    {
        int now = Interlocked.Increment(ref active);
        int old;
        while (now > (old = Volatile.Read(ref peak)) && Interlocked.CompareExchange(ref peak, now, old) != old)
        {
        }
        await Task.Delay(10); // no thread is held while the task waits
        Interlocked.Decrement(ref active);
        return n;
    }
    finally
    {
        permits.Release();
    }
}

int[] results = await Task.WhenAll(Enumerable.Range(0, Tasks).Select(Work));
Console.WriteLine($"tasks: {results.Length}, sum: {results.Sum()}");
Console.WriteLine($"never more than 10 at once: {Volatile.Read(ref peak) <= 10}");
```

```output for=async-limit
tasks: 1000, sum: 499500
never more than 10 at once: True
```

- Verdict: absent. .NET has no virtual threads. `async` methods are the alternative: at each `await`, the method gives its pool thread back until the awaited operation completes.
- `SemaphoreSlim.WaitAsync` waits for a permit without a blocked thread. Ten permits allow at most ten tasks in the delay at the same time.
- The 1,000 tasks need only a few pool threads, because a task holds no thread while it waits.
- Unlike a virtual thread, an `async` method must mark each point where it can wait with `await`.
