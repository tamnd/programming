```csharp run id=bounded-and-unbounded-channels
using System.Threading.Channels;

// A consumer task owns the total; producers send it numbers through a channel.
var channel = Channel.CreateUnbounded<int>();
var consumer = Task.Run(async () =>
{
    int total = 0;
    await foreach (int n in channel.Reader.ReadAllAsync())
    {
        total += n;
    }
    return total;
});
var producers = Enumerable.Range(0, 4).Select(_ => Task.Run(async () =>
{
    for (int k = 0; k < 1000; k++)
    {
        await channel.Writer.WriteAsync(1);
    }
})).ToArray();
await Task.WhenAll(producers);
channel.Writer.Complete();
Console.WriteLine($"total {await consumer}");

// A bounded channel in Wait mode refuses TryWrite when it is full.
var small = Channel.CreateBounded<string>(new BoundedChannelOptions(2) { FullMode = BoundedChannelFullMode.Wait });
Console.WriteLine($"{small.Writer.TryWrite("a")} {small.Writer.TryWrite("b")} {small.Writer.TryWrite("c")} {small.Reader.Count}");

// DropOldest makes room by discarding the oldest message.
var latest = Channel.CreateBounded<int>(new BoundedChannelOptions(2) { FullMode = BoundedChannelFullMode.DropOldest });
foreach (int i in new[] { 1, 2, 3, 4 })
{
    latest.Writer.TryWrite(i);
}
latest.Writer.Complete();
var kept = new List<int>();
await foreach (int i in latest.Reader.ReadAllAsync())
{
    kept.Add(i);
}
Console.WriteLine(string.Join(" ", kept));

// After Complete, a write fails.
Console.WriteLine(latest.Writer.TryWrite(5));
```

```output for=bounded-and-unbounded-channels
total 4000
True True False 2
3 4
False
```

- Verdict: stdlib. Four producer tasks write to an unbounded `Channel<int>`, and one consumer reads with `ReadAllAsync` until `Complete`. A bounded channel in `Wait` mode refuses the third `TryWrite`. In `DropOldest` mode it keeps the last two of four messages. A write after `Complete` fails.
