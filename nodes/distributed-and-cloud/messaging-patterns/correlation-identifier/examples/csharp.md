```csharp run id=match-replies
using System.Text.Json;

var prices = new Dictionary<string, int> { ["A1"] = 1999, ["B2"] = 500 };

// One reply channel for all the requests of this requestor.
var replyChannel = new List<byte[]>
{
    JsonSerializer.SerializeToUtf8Bytes(new Reply("q0", "1999 cents")), // left from a request that timed out
};

var pending = new Dictionary<string, string>(); // request id -> what the request asked for
var requests = new List<byte[]>();
foreach (var r in new[] { new Request("q1", "A1"), new Request("q2", "B2"), new Request("q3", "Z9") })
{
    pending[r.Id] = r.Sku;
    requests.Add(JsonSerializer.SerializeToUtf8Bytes(r));
}

// The replier works on the requests in a different order, and it copies the id of each request.
for (var i = requests.Count - 1; i >= 0; i--)
{
    var r = JsonSerializer.Deserialize<Request>(requests[i])!;
    var body = prices.TryGetValue(r.Sku, out var p) ? $"{p} cents" : $"error unknown sku {r.Sku}";
    replyChannel.Add(JsonSerializer.SerializeToUtf8Bytes(new Reply(r.Id, body)));
}

foreach (var raw in replyChannel)
{
    var reply = JsonSerializer.Deserialize<Reply>(raw)!;
    if (!pending.Remove(reply.CorrelationId, out var sku))
    {
        Console.WriteLine($"no outstanding request for {reply.CorrelationId}, dropped");
        continue;
    }
    Console.WriteLine($"reply for {reply.CorrelationId} ({sku}): {reply.Body}");
}
Console.WriteLine($"still waiting: {pending.Count}");

record Request(string Id, string Sku);

record Reply(string CorrelationId, string Body);
```

```output
no outstanding request for q0, dropped
reply for q3 (Z9): error unknown sku Z9
reply for q2 (B2): 500 cents
reply for q1 (A1): 1999 cents
still waiting: 0
```

- Verdict: idiom. A `Dictionary` from the request id to the item is the table, and `System.Text.Json` gives the bytes of each message.
- `Dictionary.Remove(key, out value)` finds and removes the entry in one call, and returns `false` for a stray reply.
- MassTransit copies a `RequestId` from each request to its response, so its request client does this matching for the program.
