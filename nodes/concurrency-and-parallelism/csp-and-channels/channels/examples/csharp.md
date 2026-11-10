```csharp run id=channel
using System.Threading.Channels;

var jobs = Channel.CreateBounded<int>(2); // full mode Wait: WriteAsync waits for room
var producers = Enumerable.Range(1, 2).Select(id => Task.Run(async () =>
{
    for (int i = 1; i <= 3; i++)
    {
        await jobs.Writer.WriteAsync(id * 10 * i);
    }
})).ToArray();
_ = Task.WhenAll(producers).ContinueWith(_ => jobs.Writer.Complete()); // after all producers

int total = 0;
await foreach (int v in jobs.Reader.ReadAllAsync()) // ends after Complete
{
    total += v;
}
Console.WriteLine($"total: {total}");
Console.WriteLine($"TryWrite after Complete: {jobs.Writer.TryWrite(1)}");

try
{
    await jobs.Writer.WriteAsync(1);
}
catch (ChannelClosedException e)
{
    Console.WriteLine(e.GetType().Name);
}

using var limit = new CancellationTokenSource(TimeSpan.FromMilliseconds(50));
var empty = Channel.CreateUnbounded<int>();
try
{
    await empty.Reader.ReadAsync(limit.Token);
}
catch (OperationCanceledException)
{
    Console.WriteLine("timeout");
}
```

```output for=channel
total: 180
TryWrite after Complete: False
ChannelClosedException
timeout
```

- Verdict: stdlib. `System.Threading.Channels` gives `Channel<T>` with a writer half and a reader half. Its operations are asynchronous: `WriteAsync` and `ReadAsync` return a `ValueTask`.
- A bounded channel needs a capacity greater than zero, so there is no rendezvous channel.
- `Complete` closes the writer. A later write throws `ChannelClosedException`, and the reader sees the end after the buffered items.
- `BlockingCollection<T>` is the older, blocking form: `Add`, `Take`, `CompleteAdding` and `GetConsumingEnumerable`.
