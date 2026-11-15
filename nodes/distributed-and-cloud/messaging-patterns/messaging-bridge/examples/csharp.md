```csharp run id=bridge-queues
using System;
using System.Collections.Generic;
using System.Linq;

const int Limit = 64 * 1024;
var routes = new Dictionary<string, string> { ["orders"] = "sb-orders", ["invoices"] = "sb-invoices" };
var source = new Queue<SourceMsg>(new[] {
    new SourceMsg("m1", "orders", "new-order", 120), new SourceMsg("m2", "invoices", "invoice", 300),
    new SourceMsg("m3", "orders", "new-order", 90), new SourceMsg("m4", "orders", "bulk-order", 70000),
    new SourceMsg("m5", "invoices", "invoice", 210) });
var dest = new Dictionary<string, List<DestMsg>> { ["sb-orders"] = new(), ["sb-invoices"] = new() };
var errors = new List<string>();
var forwarded = new HashSet<string>();
bool crashed = false;

while (source.TryPeek(out var m)) // the message stays on the source until the ack
{
    if (m.Size > Limit)
    {
        Console.WriteLine($"{m.Id} too large for the destination ({m.Size} > {Limit} bytes): to the bridge error queue");
        errors.Add(m.Id);
    }
    else if (forwarded.Contains(m.Id))
    {
        Console.WriteLine($"{m.Id} again: already forwarded, ack only");
    }
    else
    {
        var target = routes[m.Channel];
        dest[target].Add(new DestMsg(m.Id, m.Label, m.Size));
        forwarded.Add(m.Id);
        Console.WriteLine($"{m.Id} {m.Channel} -> {target}, {m.Size} bytes, subject {m.Label}");
        if (m.Id == "m3" && !crashed)
        {
            crashed = true;
            Console.WriteLine("the bridge stops before the ack of m3; the source delivers m3 again");
            continue;
        }
    }
    source.Dequeue(); // the ack
}

var summary = string.Join(" | ", dest.Select(kv => $"{kv.Key}: {string.Join(" ", kv.Value.Select(x => x.Id))}"));
Console.WriteLine($"{summary} | bridge errors: {string.Join(" ", errors)}");

record SourceMsg(string Id, string Channel, string Label, int Size);
record DestMsg(string Id, string Subject, int Size);
```

```output
m1 orders -> sb-orders, 120 bytes, subject new-order
m2 invoices -> sb-invoices, 300 bytes, subject invoice
m3 orders -> sb-orders, 90 bytes, subject new-order
the bridge stops before the ack of m3; the source delivers m3 again
m3 again: already forwarded, ack only
m4 too large for the destination (70000 > 65536 bytes): to the bridge error queue
m5 invoices -> sb-invoices, 210 bytes, subject invoice
sb-orders: m1 m3 | sb-invoices: m2 m5 | bridge errors: m4
```

- Verdict: idiom. `Queue<T>.TryPeek` reads the head without removing it, and `Dequeue` after the forward is the ack.
- A `HashSet<string>` is the de-dup store, and `continue` before the `Dequeue` plays the stop of the bridge.
- Records hold the source and the destination formats.
