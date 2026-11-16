```csharp run id=async-delay
using System;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

const int Count = 10_000;

static async Task<int> Work(int i)
{
    await Task.Delay(100); // no thread is blocked while the delay runs
    return i;
}

var clock = Stopwatch.StartNew();
int[] results = await Task.WhenAll(Enumerable.Range(0, Count).Select(Work));
clock.Stop();
Console.WriteLine($"tasks: {results.Length}, sum: {results.Sum(i => (long)i)}");
Console.WriteLine($"all waited 100 ms in under 5 s: {clock.Elapsed < TimeSpan.FromSeconds(5)}");
```

```output for=async-delay
tasks: 10000, sum: 49995000
all waited 100 ms in under 5 s: True
```

- Verdict: absent. .NET threads are OS threads. `async` methods and tasks are the alternative: an `await` gives the thread back to the thread pool until the awaited operation completes.
- 10,000 tasks each wait 100 ms. They wait at the same time, so the whole run takes far less than 10,000 times 100 ms, and it needs only a few pool threads.
- `Task.WhenAll` waits for all tasks and gives their results in the order of the tasks.
- A task that blocks a thread, for example with `Thread.Sleep`, holds a pool thread for the whole wait.
