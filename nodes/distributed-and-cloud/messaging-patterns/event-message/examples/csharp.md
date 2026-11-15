```csharp run id=order-events
using System.Text.Json;

// Each subscriber gets a copy of each event.
var billing = new Subscriber("billing", new()
{
    ["com.example.order.placed"] = d => $"charge order {d["order"]}, {d["cents"]} cents",
    ["com.example.order.cancelled"] = d => $"refund order {d["order"]}",
});
var email = new Subscriber("email", new()
{
    ["com.example.order.placed"] = d => $"mail the confirmation of order {d["order"]}",
});
Subscriber[] topic = [billing, email];

byte[][] events =
[
    Event("e1", "placed", new() { ["order"] = 41, ["cents"] = 1999 }),
    Event("e2", "placed", new() { ["order"] = 42, ["cents"] = 500 }),
    Event("e2", "placed", new() { ["order"] = 42, ["cents"] = 500 }), // sent again after a network error
    Event("e3", "cancelled", new() { ["order"] = 41 }),
];
foreach (var raw in events)
{
    foreach (var s in topic)
    {
        s.Deliver(raw);
    }
}

static byte[] Event(string id, string type, Dictionary<string, int> data) =>
    JsonSerializer.SerializeToUtf8Bytes(new CloudEvent("1.0", id, "/shop", "com.example.order." + type, data));

// An event message with the required CloudEvents attributes and the data of the event.
record CloudEvent(string specversion, string id, string source, string type, Dictionary<string, int> data);

class Subscriber(string name, Dictionary<string, Func<Dictionary<string, int>, string>> handlers)
{
    private readonly HashSet<(string, string)> seen = [];

    public void Deliver(byte[] raw)
    {
        var e = JsonSerializer.Deserialize<CloudEvent>(raw)!;
        if (!seen.Add((e.source, e.id)))
        {
            Console.WriteLine($"{name}: skip duplicate {e.id}");
            return;
        }
        if (!handlers.TryGetValue(e.type, out var handler))
        {
            Console.WriteLine($"{name}: ignore {e.type}");
            return;
        }
        Console.WriteLine($"{name}: {handler(e.data)}");
    }
}
```

```output
billing: charge order 41, 1999 cents
email: mail the confirmation of order 41
billing: charge order 42, 500 cents
email: mail the confirmation of order 42
billing: skip duplicate e2
email: skip duplicate e2
billing: refund order 41
email: ignore com.example.order.cancelled
```

- Verdict: idiom. .NET has no publish-subscribe channel in the base library. An array of subscribers is the topic, and `System.Text.Json` turns each event into UTF-8 bytes and back.
- The record `CloudEvent` has lowercase property names, so the JSON names are the CloudEvents names with no extra options.
- `HashSet.Add` with a tuple of the source and the id returns `false` for a duplicate. Libraries such as MassTransit give `Publish` over a broker.
