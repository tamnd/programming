```csharp run id=active
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

// The servant: only the scheduler thread calls it, so it has no lock.
class AccountServant
{
    private int total;
    public int Deposit(int amount) => total += amount;
    public int Balance() => total;
}

// The proxy: each method makes a request and returns a future at once.
class AccountProxy
{
    private readonly AccountServant servant = new();
    private readonly BlockingCollection<Action> activationQueue = new();
    private readonly Thread scheduler;
    public readonly ManualResetEventSlim Gate = new(false); // lets the demo hold the scheduler

    public AccountProxy()
    {
        scheduler = new Thread(() =>
        {
            Gate.Wait();
            foreach (var request in activationQueue.GetConsumingEnumerable()) request();
        });
        scheduler.Start();
    }

    private Task<int> Send(Func<AccountServant, int> call)
    {
        var future = new TaskCompletionSource<int>();
        activationQueue.Add(() => future.SetResult(call(servant)));
        return future.Task;
    }

    public Task<int> Deposit(int amount) => Send(s => s.Deposit(amount));
    public Task<int> Balance() => Send(s => s.Balance());

    public void Stop()
    {
        activationQueue.CompleteAdding();
        scheduler.Join();
    }
}

static class Program
{
    static void Main()
    {
        var account = new AccountProxy();
        var perClient = Enumerable.Range(0, 4).Select(_ => new List<Task<int>>()).ToList();
        var clients = perClient.Select(mine => new Thread(() =>
        {
            for (int i = 0; i < 250; i++) mine.Add(account.Deposit(1)); // returns at once
        })).ToList();
        clients.ForEach(t => t.Start());
        clients.ForEach(t => t.Join());
        var futures = perClient.SelectMany(mine => mine).ToList();
        Console.WriteLine("deposits queued: " + futures.Count);
        Console.WriteLine("deposits done before the scheduler starts: " + futures.Count(f => f.IsCompleted));
        account.Gate.Set();
        Console.WriteLine("balance after 1000 deposits: " + account.Balance().Result);
        account.Stop();
    }
}
```

```output
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

- Verdict: library. `BlockingCollection` is the activation queue and `TaskCompletionSource` makes the future. The scheduler is a plain thread.
