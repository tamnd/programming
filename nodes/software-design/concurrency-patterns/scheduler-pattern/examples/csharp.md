```csharp run id=scheduler-pattern
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

// Decides which waiting thread uses the printer next.
class Scheduler
{
    record Entry(int Priority, int Arrival, string Name);

    readonly object gate = new object();
    bool busy = false;
    readonly List<Entry> waiting = new List<Entry>();
    int arrivals = 0;
    Entry chosen = null;

    public void Enter(string name, int priority)
    {
        lock (gate)
        {
            if (!busy && waiting.Count == 0)
            {
                busy = true;
                return;
            }
            var entry = new Entry(priority, ++arrivals, name);
            waiting.Add(entry);
            Monitor.PulseAll(gate);
            // Sleep until the scheduler chooses this thread.
            while (!ReferenceEquals(chosen, entry)) Monitor.Wait(gate);
            chosen = null;
        }
    }

    public void Done()
    {
        lock (gate)
        {
            if (waiting.Count == 0)
            {
                busy = false;
                return;
            }
            // The policy: the lowest priority number first, then the first to arrive.
            var entry = waiting.OrderBy(e => e.Priority).ThenBy(e => e.Arrival).First();
            waiting.Remove(entry);
            chosen = entry; // the printer stays busy and passes to that thread
            Monitor.PulseAll(gate);
        }
    }

    public int WaitUntilWaiting(int n)
    {
        lock (gate)
        {
            var end = DateTime.UtcNow.AddSeconds(5);
            while (waiting.Count < n && DateTime.UtcNow < end) Monitor.Wait(gate, 100);
            return waiting.Count;
        }
    }
}

class Program
{
    static readonly Scheduler scheduler = new Scheduler();
    static readonly List<string> order = new List<string>();
    static int onPrinter = 0, mostOnPrinter = 0;

    static void UsePrinter(string name)
    {
        int now = Interlocked.Increment(ref onPrinter);
        lock (order)
        {
            mostOnPrinter = Math.Max(mostOnPrinter, now);
            order.Add(name);
        }
        Interlocked.Decrement(ref onPrinter);
    }

    static void Main()
    {
        scheduler.Enter("first", 0); // the first job takes the free printer
        UsePrinter("first");
        var jobs = new (string Name, int Priority)[] { ("report", 2), ("invoice", 1), ("photo", 3), ("label", 1) };
        var threads = new List<Thread>();
        for (int i = 0; i < jobs.Length; i++)
        {
            var job = jobs[i];
            var t = new Thread(() =>
            {
                scheduler.Enter(job.Name, job.Priority);
                UsePrinter(job.Name);
                scheduler.Done();
            });
            t.Start();
            threads.Add(t);
            scheduler.WaitUntilWaiting(i + 1); // start the jobs in a known order
        }
        Console.WriteLine("jobs waiting when the first job ends: " + scheduler.WaitUntilWaiting(jobs.Length));
        scheduler.Done();
        foreach (var t in threads) t.Join();

        Console.WriteLine("print order: " + string.Join(", ", order));
        Console.WriteLine("jobs on the printer at once, at most: " + mostOnPrinter);
    }
}
```

```output
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

- Verdict: idiom. A class with `lock`, `Monitor.Wait` in a `while` loop and `Monitor.PulseAll`.
