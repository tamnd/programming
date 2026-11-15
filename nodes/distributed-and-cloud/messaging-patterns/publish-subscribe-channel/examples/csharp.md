```csharp run id=three-subscribers
using System.Threading.Channels;

var topic = new Topic();
var names = new[] { "billing", "email", "stock" };
var readers = names.Select(name =>
{
    var inbox = topic.Subscribe(name);
    return Task.Run(async () =>
    {
        var got = new List<string>();
        await foreach (var e in inbox.ReadAllAsync())
        {
            got.Add(e);
        }
        return (name, got);
    });
}).ToArray();

var copies = 0;
for (var n = 1; n <= 3; n++)
{
    copies += topic.Publish($"order {n} placed");
}
topic.Complete();

foreach (var (name, got) in await Task.WhenAll(readers))
{
    Console.WriteLine($"{name}: {string.Join(", ", got)}");
}
Console.WriteLine($"copies sent: {copies}");

// A publish-subscribe channel: one channel for each subscriber.
sealed class Topic
{
    private readonly List<Channel<string>> _subs = new();

    public ChannelReader<string> Subscribe(string name)
    {
        var ch = Channel.CreateUnbounded<string>();
        _subs.Add(ch);
        return ch.Reader;
    }

    public int Publish(string e)
    {
        foreach (var ch in _subs)
        {
            ch.Writer.TryWrite(e); // each subscriber gets its own copy
        }
        return _subs.Count;
    }

    public void Complete()
    {
        foreach (var ch in _subs)
        {
            ch.Writer.Complete();
        }
    }
}
```

```output
billing: order 1 placed, order 2 placed, order 3 placed
email: order 1 placed, order 2 placed, order 3 placed
stock: order 1 placed, order 2 placed, order 3 placed
copies sent: 9
```

- Verdict: stdlib. `Topic` keeps one unbounded `Channel<string>` for each subscriber, and `Publish` writes the event to each.
- Each reader task returns its name and its list, and `Task.WhenAll` gives the results in the order of the tasks, so the output is stable.
- With Azure Service Bus, the publisher sends to a topic, and each subscriber receives from its own subscription of the topic.
