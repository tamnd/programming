```csharp run id=executor-policies
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

// A scheduler that runs each task at once, on the thread that queues it.
sealed class InlineScheduler : TaskScheduler
{
    protected override void QueueTask(Task task) => TryExecuteTask(task);

    protected override bool TryExecuteTaskInline(Task task, bool taskWasPreviouslyQueued) =>
        TryExecuteTask(task);

    protected override IEnumerable<Task> GetScheduledTasks() => [];
}

static class Program
{
    static int running;
    static int maxRunning;

    // This code knows only the TaskScheduler. It returns the IDs of the threads that ran the tasks.
    static HashSet<int> ThreadsUsed(TaskScheduler scheduler, int count)
    {
        running = 0;
        maxRunning = 0;
        var tasks = Enumerable.Range(0, count).Select(_ => Task.Factory.StartNew(() =>
        {
            int now = Interlocked.Increment(ref running);
            int seen;
            while (now > (seen = Volatile.Read(ref maxRunning)) &&
                   Interlocked.CompareExchange(ref maxRunning, now, seen) != seen) { }
            Thread.Sleep(20);
            Interlocked.Decrement(ref running);
            return Environment.CurrentManagedThreadId;
        }, CancellationToken.None, TaskCreationOptions.None, scheduler)).ToArray();
        Task.WaitAll(tasks);
        return tasks.Select(t => t.Result).ToHashSet();
    }

    static void Main()
    {
        int caller = Environment.CurrentManagedThreadId;

        var inline = ThreadsUsed(new InlineScheduler(), 4);
        Console.WriteLine($"inline: ran on the caller's thread only: {inline.SetEquals([caller])}");

        var pool = ThreadsUsed(TaskScheduler.Default, 4);
        Console.WriteLine($"default: used the caller's thread: {pool.Contains(caller)}");

        var pair = new ConcurrentExclusiveSchedulerPair();
        ThreadsUsed(pair.ExclusiveScheduler, 4);
        Console.WriteLine($"exclusive: most tasks at the same time: {maxRunning}");
    }
}
```

```output for=executor-policies
inline: ran on the caller's thread only: True
default: used the caller's thread: False
exclusive: most tasks at the same time: 1
```

- Verdict: stdlib. `TaskScheduler` is the abstract class that puts tasks on threads. `TaskScheduler.Default` uses the .NET thread pool.
- `ThreadsUsed` starts the same tasks on each scheduler. It does not know how the scheduler runs them.
- `InlineScheduler` runs each task in its `QueueTask` call, on the thread that started it.
- `ConcurrentExclusiveSchedulerPair` gives two schedulers. Tasks on its `ExclusiveScheduler` never run at the same time, so the most that run together is 1.
- Most code uses `Task.Run` and `async`/`await`, which use the default scheduler. A custom scheduler is a rare tool.
