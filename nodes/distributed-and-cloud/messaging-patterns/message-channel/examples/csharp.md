```csharp run id=named-channels
using System.Threading.Channels;

var channels = new NamedChannels("orders", "payments");
channels.Send("orders", "order 1: 2 x A-7");
channels.Send("orders", "order 2: 1 x B-1");
channels.Send("payments", "payment for order 1: 30.00");
int orders = channels.Depth("orders"), payments = channels.Depth("payments");
Console.WriteLine($"waiting: orders={orders} payments={payments}");
channels.Complete();

foreach (var (name, who) in new[] { ("orders", "warehouse"), ("payments", "accounting") })
{
    await Task.Run(async () =>
    {
        await foreach (var m in channels.Reader(name).ReadAllAsync())
        {
            Console.WriteLine($"{who}: {m}");
        }
    });
}

try
{
    channels.Send("order", "order 3: 1 x C-2");
}
catch (KeyNotFoundException e)
{
    Console.WriteLine($"error: {e.Message}");
}

// Named channels. Senders and receivers share only the names.
sealed class NamedChannels
{
    private readonly Dictionary<string, Channel<string>> _byName = new();

    public NamedChannels(params string[] names)
    {
        foreach (var n in names)
        {
            _byName[n] = Channel.CreateUnbounded<string>();
        }
    }

    private Channel<string> Get(string name) =>
        _byName.TryGetValue(name, out var ch) ? ch : throw new KeyNotFoundException($"no channel named {name}");

    public void Send(string name, string message)
    {
        if (!Get(name).Writer.TryWrite(message))
        {
            throw new InvalidOperationException($"channel {name} is closed");
        }
    }

    public ChannelReader<string> Reader(string name) => Get(name).Reader;

    public int Depth(string name) => Get(name).Reader.Count;

    public void Complete()
    {
        foreach (var ch in _byName.Values)
        {
            ch.Writer.Complete();
        }
    }
}
```

```output
waiting: orders=2 payments=1
warehouse: order 1: 2 x A-7
warehouse: order 2: 1 x B-1
accounting: payment for order 1: 30.00
error: no channel named order
```

- Verdict: stdlib. `Channel.CreateUnbounded<string>()` from `System.Threading.Channels` makes a channel with a writer side and a reader side, and `NamedChannels` maps the names to the channels.
- `Complete` tells the readers that no more messages will come, and `ReadAllAsync` then ends. A broker client, such as the Azure Service Bus client, has a sender and a receiver for each queue or topic name in the same way.
- `Reader.Count` is available because an unbounded channel can count its items.
