```csharp run id=producer-consumer
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

class Program
{
    static void Main()
    {
        var buffer = new BlockingCollection<int>(boundedCapacity: 4);
        var consumed = new ConcurrentBag<int>();
        var producers = new[] { 0, 500 }.Select(s => new Thread(() =>
        {
            for (int item = s; item < s + 500; item++) buffer.Add(item);
        })).ToList();
        var consumers = Enumerable.Range(0, 2).Select(_ => new Thread(() =>
        {
            foreach (var item in buffer.GetConsumingEnumerable()) consumed.Add(item);
        })).ToList();
        foreach (var t in producers.Concat(consumers)) t.Start();
        foreach (var t in producers) t.Join();
        buffer.CompleteAdding(); // the end of the stream for every consumer
        foreach (var t in consumers) t.Join();

        var sorted = consumed.OrderBy(x => x).ToList();
        Console.WriteLine("items consumed: " + sorted.Count);
        Console.WriteLine("sum of items: " + sorted.Sum(x => (long)x));
        Console.WriteLine("each item consumed once: " + sorted.SequenceEqual(Enumerable.Range(0, 1000)));
    }
}
```

```output
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

- Verdict: stdlib. `BlockingCollection` with `boundedCapacity: 4` is the bounded buffer, and `CompleteAdding` ends the stream instead of an end-of-stream object.
