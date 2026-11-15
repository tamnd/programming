```csharp run id=route-orders
using System.Text.Json;

// The router reads one input channel and writes to the channel of the system for each item type.
var channels = new Dictionary<string, List<string>>
{
    ["inventory.widgets"] = [],
    ["inventory.gadgets"] = [],
    ["invalid"] = [],
};

Order[] incoming = [new("o1", "widget", 3), new("o2", "gadget", 1), new("o3", "gizmo", 2), new("o4", "widget", 1)];
foreach (var o in incoming)
{
    Route(JsonSerializer.SerializeToUtf8Bytes(o));
}
foreach (var (name, ids) in channels)
{
    Console.WriteLine($"{name}: {string.Join(" ", ids)}");
}

void Route(byte[] raw)
{
    var o = JsonSerializer.Deserialize<Order>(raw)!;
    var channel = o.Type switch
    {
        "widget" => "inventory.widgets",
        "gadget" => "inventory.gadgets",
        _ => "invalid", // an unknown type is not lost
    };
    channels[channel].Add(o.Id);
}

record Order(string Id, string Type, int Qty);
```

```output
inventory.widgets: o1 o4
inventory.gadgets: o2
invalid: o3
```

- Verdict: idiom. A `switch` expression on the item type is the rule table, and the discard arm `_` is the default rule.
- The compiler warns when a `switch` expression has no arm for some values, which helps to keep a default.
- `System.Text.Json` turns each order into bytes and back, as on a broker.
