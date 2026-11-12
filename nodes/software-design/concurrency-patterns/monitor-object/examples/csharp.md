```csharp run id=monitor
using System;
using System.Collections.Generic;
using System.Threading;

class MessageQueue
{
    private readonly object monitor = new object();
    private readonly Queue<int> items = new Queue<int>();
    private readonly int capacity;
    public int Puts, Gets, Largest;

    public MessageQueue(int capacity) => this.capacity = capacity;

    public void Put(int message)
    {
        lock (monitor)
        {
            while (items.Count == capacity)
                Monitor.Wait(monitor); // releases the lock while it waits
            items.Enqueue(message);
            Puts++;
            Largest = Math.Max(Largest, items.Count);
            Monitor.PulseAll(monitor);
        }
    }

    public int Get()
    {
        lock (monitor)
        {
            while (items.Count == 0)
                Monitor.Wait(monitor);
            int message = items.Dequeue();
            Gets++;
            Monitor.PulseAll(monitor);
            return message;
        }
    }
}

class Program
{
    static void Main()
    {
        var q = new MessageQueue(2);
        long[] sums = new long[2];
        var threads = new List<Thread>();
        for (int p = 0; p < 4; p++)
        {
            int start = p * 250;
            threads.Add(new Thread(() => { for (int i = 0; i < 250; i++) q.Put(start + i); }));
        }
        for (int c = 0; c < 2; c++)
        {
            int slot = c;
            threads.Add(new Thread(() => { for (int i = 0; i < 500; i++) sums[slot] += q.Get(); }));
        }
        threads.ForEach(t => t.Start());
        threads.ForEach(t => t.Join());
        Console.WriteLine("messages put: " + q.Puts);
        Console.WriteLine("messages got: " + q.Gets);
        Console.WriteLine("sum of messages got: " + (sums[0] + sums[1]));
        Console.WriteLine("queue never held more than 2: " + (q.Largest <= 2 ? "True" : "False"));
    }
}
```

```output
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

- Verdict: stdlib. The `lock` statement calls `Monitor.Enter` and `Monitor.Exit`, and `Monitor.Wait` and `Monitor.PulseAll` give one wait queue per lock object.
