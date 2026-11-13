```csharp run id=worker-queue
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

// Hands out tasks and keeps each one until a worker acknowledges it.
class WorkQueue
{
    readonly object gate = new object();
    readonly Queue<int> ready = new Queue<int>();
    readonly Dictionary<int, int> unacked = new Dictionary<int, int>(); // delivery tag -> task
    int tag;
    public int Pending; // tasks put and not yet acknowledged
    public int Deliveries;
    public int PutBack;

    public void Put(int task)
    {
        lock (gate)
        {
            ready.Enqueue(task);
            Pending++;
            Monitor.PulseAll(gate);
        }
    }

    // Returns (tag, task), or null when every task is acknowledged.
    public (int Tag, int Task)? Get()
    {
        lock (gate)
        {
            while (ready.Count == 0 && Pending > 0) Monitor.Wait(gate, 5000);
            if (ready.Count == 0) return null;
            tag++;
            Deliveries++;
            unacked[tag] = ready.Dequeue();
            return (tag, unacked[tag]);
        }
    }

    public void Ack(int t)
    {
        lock (gate)
        {
            unacked.Remove(t);
            Pending--;
            Monitor.PulseAll(gate);
        }
    }

    // The worker went away without an ack, so the task goes back on the queue.
    public void WorkerLost(int t)
    {
        lock (gate)
        {
            ready.Enqueue(unacked[t]);
            unacked.Remove(t);
            PutBack++;
            Monitor.PulseAll(gate);
        }
    }
}

class Program
{
    const int Tasks = 10;
    const int Workers = 3;
    const int FailsOn = 4; // the first worker that takes this task fails before it does the work

    static readonly WorkQueue work = new WorkQueue();
    static readonly SortedDictionary<int, int> results = new SortedDictionary<int, int>();
    static bool failed;

    static void Worker()
    {
        while (work.Get() is (int tag, int task))
        {
            bool fail;
            lock (results)
            {
                fail = task == FailsOn && !failed;
                if (fail) failed = true;
            }
            if (fail)
            {
                work.WorkerLost(tag); // this worker stops here
                return;
            }
            lock (results) results[task] = task * task; // the work
            work.Ack(tag); // only after the work is done
        }
    }

    static void Main()
    {
        for (int task = 1; task <= Tasks; task++) work.Put(task);
        var threads = Enumerable.Range(0, Workers).Select(_ => new Thread(Worker)).ToList();
        foreach (var t in threads) t.Start();
        foreach (var t in threads) t.Join();

        bool all = results.Keys.SequenceEqual(Enumerable.Range(1, Tasks));
        Console.WriteLine("tasks put on the queue: " + Tasks);
        Console.WriteLine("workers: " + Workers);
        Console.WriteLine("deliveries: " + work.Deliveries);
        Console.WriteLine("tasks put back after a worker failed: " + work.PutBack);
        Console.WriteLine("tasks still not acknowledged: " + work.Pending);
        Console.WriteLine("every task has a result: " + (all ? "True" : "False"));
    }
}
```

```output
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

- Verdict: idiom. A `WorkQueue` class on `lock`, `Monitor.Wait` and `Monitor.PulseAll`, with a nullable tuple for the delivery.
