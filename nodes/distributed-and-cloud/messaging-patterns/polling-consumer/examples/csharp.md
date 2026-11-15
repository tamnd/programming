```csharp run id=poll-orders
using System.Collections.Concurrent;

var orders = new BlockingCollection<string>();

// Wait up to timeoutMs for one message, then take up to max messages.
List<string> Poll(int max, int timeoutMs)
{
    var batch = new List<string>();
    if (!orders.TryTake(out var first, timeoutMs)) return batch;
    batch.Add(first);
    while (batch.Count < max && orders.TryTake(out var next)) batch.Add(next);
    return batch;
}

Console.WriteLine("poll 0, no wait: " + (Poll(2, 0).Count == 0 ? "empty" : "?"));
for (int n = 1; n <= 5; n++) orders.Add($"order-{n}");
Console.WriteLine("producer: 5 orders on the queue");

Timer? late = null;
for (int n = 1; ; n++)
{
    if (n == 4)
    {
        // a producer sends one more order while the consumer waits
        late = new Timer(_ => orders.Add("order-6"), null, 50, Timeout.Infinite);
    }
    var batch = Poll(2, 300);
    if (batch.Count == 0)
    {
        Console.WriteLine($"poll {n}: nothing in 300 ms, the consumer stops");
        break;
    }
    Console.WriteLine($"poll {n}: {string.Join(" ", batch)} ({orders.Count} left)");
    // the consumer works on the batch here, at its own pace
}
late?.Dispose();
```

```output
poll 0, no wait: empty
producer: 5 orders on the queue
poll 1: order-1 order-2 (3 left)
poll 2: order-3 order-4 (1 left)
poll 3: order-5 (0 left)
poll 4: order-6 (0 left)
poll 5: nothing in 300 ms, the consumer stops
```

- Verdict: idiom. `BlockingCollection<T>.TryTake(out item, timeout)` waits up to the timeout, and with a timeout of 0 it returns at once.
- `TryTake(out next)` with no timeout fills the rest of the batch from what is already there.
- A `System.Threading.Timer` plays a producer that sends while the consumer waits in poll 4.
