```csharp run id=three-ways
using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Threading;

class Program
{
    const int Threads = 4;
    const int Steps = 1000;

    static void RunThreads(Action<int> body)
    {
        var threads = Enumerable.Range(0, Threads).Select(i => new Thread(() => body(i))).ToList();
        foreach (var t in threads) t.Start();
        foreach (var t in threads) t.Join();
    }

    static void Main()
    {
        // 1. Synchronization: a lock guards the shared counter.
        int count = 0;
        var gate = new object();
        RunThreads(_ =>
        {
            for (int n = 0; n < Steps; n++)
                lock (gate) count++;
        });
        Console.WriteLine("lock around a shared counter: " + count);

        // 2. Confinement: one owner thread keeps the counter; the others send it messages.
        var inbox = new BlockingCollection<int>();
        int owned = 0;
        var owner = new Thread(() =>
        {
            int total = 0;
            for (int n = 0; n < Threads * Steps; n++) total += inbox.Take();
            owned = total;
        });
        owner.Start();
        RunThreads(_ =>
        {
            for (int n = 0; n < Steps; n++) inbox.Add(1);
        });
        owner.Join();
        Console.WriteLine("one owner thread, the others send messages: " + owned);

        // 3. No sharing: each thread makes its own result, and the results are combined after the join.
        var parts = new int[Threads];
        RunThreads(i =>
        {
            for (int n = 0; n < Steps; n++) parts[i]++; // only thread i writes slot i
        });
        Console.WriteLine("own result for each thread, combined after the join: " + parts.Sum());
    }
}
```

```output
lock around a shared counter: 4000
one owner thread, the others send messages: 4000
own result for each thread, combined after the join: 4000
```

- Verdict: native. The `lock` statement guards the counter, a `BlockingCollection<int>` carries the messages, and each thread writes its own array slot.
