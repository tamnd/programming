```csharp run id=thread-pool
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

class FixedThreadPool
{
    private readonly BlockingCollection<Action> queue = new BlockingCollection<Action>();
    private readonly List<Thread> threads = new List<Thread>();

    public FixedThreadPool(int size)
    {
        for (int i = 0; i < size; i++)
        {
            var t = new Thread(() =>
            {
                foreach (var work in queue.GetConsumingEnumerable())
                {
                    work();
                }
            });
            t.Start();
            threads.Add(t);
        }
    }

    public Task<T> Submit<T>(Func<T> fn)
    {
        var tcs = new TaskCompletionSource<T>();
        queue.Add(() => tcs.SetResult(fn()));
        return tcs.Task;
    }

    public void Shutdown()
    {
        queue.CompleteAdding();
        foreach (var t in threads) t.Join();
    }
}

class Program
{
    static int running = 0;
    static int mostRunning = 0;
    static readonly ConcurrentDictionary<int, bool> workers = new ConcurrentDictionary<int, bool>();

    static long Work(long n)
    {
        int now = Interlocked.Increment(ref running);
        int seen;
        while ((seen = Volatile.Read(ref mostRunning)) < now &&
               Interlocked.CompareExchange(ref mostRunning, now, seen) != seen) { }
        workers.TryAdd(Environment.CurrentManagedThreadId, true);
        Thread.Sleep(1);
        Interlocked.Decrement(ref running);
        return n * n;
    }

    static void Main()
    {
        var pool = new FixedThreadPool(4);
        var futures = new List<Task<long>>();
        for (long n = 0; n < 100; n++)
        {
            long k = n;
            futures.Add(pool.Submit(() => Work(k)));
        }
        long sum = 0;
        foreach (var f in futures) sum += f.Result;
        pool.Shutdown();
        Console.WriteLine("tasks run: " + futures.Count);
        Console.WriteLine("sum of results: " + sum);
        Console.WriteLine("workers used, at most 4: " + (workers.Count <= 4));
        Console.WriteLine("most tasks running at once, at most 4: " + (mostRunning <= 4));
    }
}
```

```output
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

- Verdict: idiom. `FixedThreadPool` is a small class: 4 threads take actions from a `BlockingCollection`, and each `Submit` returns a `Task` completed by a `TaskCompletionSource`.
