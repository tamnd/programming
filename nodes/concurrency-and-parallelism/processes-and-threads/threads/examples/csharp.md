```csharp run id=sums
using System.Threading;

long[] results = new long[4];
var workers = new Thread[4];
for (int i = 0; i < 4; i++)
{
    int index = i;
    workers[i] = new Thread(() =>
    {
        long sum = 0;
        for (long n = index * 250L; n < (index + 1) * 250L; n++)
        {
            sum += n;
        }
        results[index] = sum;
    });
}
bool background = workers[0].IsBackground;
foreach (var worker in workers)
{
    worker.Start();
}
foreach (var worker in workers)
{
    worker.Join();
}
Console.WriteLine(string.Join(", ", results));
Console.WriteLine($"background: {background}");

var slow = new Thread(() => Thread.Sleep(5000)) { IsBackground = true };
slow.Start();
bool finished = slow.Join(TimeSpan.FromMilliseconds(100));
Console.WriteLine($"finished within 100 ms: {finished}");
```

```output for=sums
31125, 93625, 156125, 218625
background: False
finished within 100 ms: False
```

- Verdict: stdlib. `System.Threading.Thread` wraps an OS thread. `Start()` runs the delegate, and `Join()` waits for it to end.
- A thread is a foreground thread by default. The process does not end while a foreground thread runs. The `slow` thread is a background thread, so the process ends without waiting for it.
- `Join(TimeSpan)` returns `false` when the time runs out first. The thread keeps running.
- For short jobs, `Task.Run` queues the work to the thread pool instead of making a dedicated thread.
