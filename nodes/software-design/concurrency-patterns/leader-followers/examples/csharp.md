```csharp run id=leader-followers
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

class ThreadPoolLF
{
    // Stands for a handle set that one thread at a time may wait on.
    public readonly BlockingCollection<int> HandleSet = new BlockingCollection<int>();
    private readonly object synchronizer = new object();
    private bool leaderPresent;
    private int waiting;
    public int MostWaiting, Processed;
    public long Sum;

    public void Join()
    {
        while (true)
        {
            lock (synchronizer) // follower role: wait for the turn to lead
            {
                while (leaderPresent) Monitor.Wait(synchronizer);
                leaderPresent = true;
                waiting++;
                MostWaiting = Math.Max(MostWaiting, waiting);
            }
            int ev = HandleSet.Take(); // leader role: wait for an event
            PromoteNewLeader();
            if (ev < 0) return;
            Thread.Sleep(1); // processing role: handle the event in this thread
            lock (synchronizer)
            {
                Processed++;
                Sum += ev;
            }
        }
    }

    void PromoteNewLeader()
    {
        lock (synchronizer)
        {
            waiting--;
            leaderPresent = false;
            Monitor.Pulse(synchronizer);
        }
    }
}

class Program
{
    static void Main()
    {
        var pool = new ThreadPoolLF();
        for (int id = 0; id < 100; id++) pool.HandleSet.Add(id);
        for (int i = 0; i < 4; i++) pool.HandleSet.Add(-1); // one stop event per thread
        var threads = new List<Thread>();
        for (int i = 0; i < 4; i++)
        {
            var t = new Thread(pool.Join);
            t.Start();
            threads.Add(t);
        }
        threads.ForEach(t => t.Join());
        Console.WriteLine("events processed: " + pool.Processed);
        Console.WriteLine("sum of event ids: " + pool.Sum);
        Console.WriteLine("most threads waiting on the handle set at once: " + pool.MostWaiting);
    }
}
```

```output
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

- Verdict: stdlib. `Monitor.Wait` holds the followers on the synchronizer, and `Monitor.Pulse` promotes one of them.
