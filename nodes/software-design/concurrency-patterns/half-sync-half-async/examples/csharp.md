```csharp run id=hsha
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

class Program
{
    static readonly BlockingCollection<int> requests = new BlockingCollection<int>(); // queueing layer
    static readonly ManualResetEventSlim gate = new ManualResetEventSlim(false);
    static int received, done;
    static long sum;

    // Async layer: a short handler that never blocks.
    static void OnEvent(int eventId)
    {
        received++;
        requests.Add(eventId);
    }

    static void Main()
    {
        var workers = new List<Thread>();
        for (int i = 0; i < 4; i++)
        {
            var w = new Thread(() =>
            {
                gate.Wait();
                foreach (int request in requests.GetConsumingEnumerable()) // blocks for each item
                {
                    Thread.Sleep(1); // a blocking call, such as a database query
                    Interlocked.Increment(ref done);
                    Interlocked.Add(ref sum, request);
                }
            });
            w.Start();
            workers.Add(w);
        }
        var eventSource = new Thread(() => { for (int id = 0; id < 100; id++) OnEvent(id); });
        eventSource.Start();
        eventSource.Join();
        Console.WriteLine("events received by the async layer: " + received);
        Console.WriteLine("requests in the queueing layer before the sync layer starts: " + requests.Count);
        gate.Set();
        requests.CompleteAdding();
        workers.ForEach(w => w.Join());
        Console.WriteLine("requests done by the sync layer: " + done);
        Console.WriteLine("sum of request ids: " + sum);
    }
}
```

```output
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

- Verdict: stdlib. `BlockingCollection<T>` is the queueing layer, and `GetConsumingEnumerable` blocks each worker until an item arrives or adding is complete.
