```csharp run id=price-requests
using System.Collections.Concurrent;

var prices = new Dictionary<string, int> { ["A1"] = 1999, ["B2"] = 500 };

// Named channels, as on a broker: one request channel and one reply channel for each requestor.
var channels = new Dictionary<string, BlockingCollection<Message>>
{
    ["prices"] = new(),
    ["replies.web"] = new(),
    ["replies.mobile"] = new(),
};

var worker = new Thread(() =>
{
    foreach (var request in channels["prices"].GetConsumingEnumerable())
    {
        var body = prices.TryGetValue(request.Body, out var p) ? $"{p} cents" : $"error unknown sku {request.Body}";
        channels[request.ReplyTo].Add(new Message("", "", request.Id, body)); // the return address
    }
});
worker.Start();

Console.WriteLine(Ask("web", "q1", "A1"));
Console.WriteLine(Ask("mobile", "q2", "B2"));
Console.WriteLine(Ask("web", "q3", "Z9"));
channels["prices"].CompleteAdding(); // stop the replier
worker.Join();

string Ask(string requestor, string id, string sku)
{
    var replyTo = "replies." + requestor;
    channels["prices"].Add(new Message(id, replyTo, "", sku));
    if (!channels[replyTo].TryTake(out var reply, TimeSpan.FromSeconds(2))) // block, but not for ever
    {
        return $"{requestor} asked for {sku}: no reply in 2 s";
    }
    return $"{requestor} asked for {sku}: {reply.Body}";
}

record Message(string Id, string ReplyTo, string CorrelationId, string Body);
```

```output
web asked for A1: 1999 cents
mobile asked for B2: 500 cents
web asked for Z9: error unknown sku Z9
```

- Verdict: idiom. .NET has no request-reply channel in the base library. A `BlockingCollection` for each channel and a thread for the replier give the pattern in one process.
- `TryTake` with a `TimeSpan` returns `false` when no reply comes in 2 seconds.
- `CompleteAdding` ends the loop of `GetConsumingEnumerable` in the replier, so the thread stops. MassTransit gives the same pattern over a broker with request clients.
