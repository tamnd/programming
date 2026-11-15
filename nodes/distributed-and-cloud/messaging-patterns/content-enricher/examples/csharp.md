```csharp run id=enrich-orders
using System;
using System.Collections.Generic;

var zipPrefix = new Dictionary<string, string> { ["981"] = "WA", ["100"] = "NY", ["606"] = "IL" };
var directory = new Dictionary<string, string> { ["c1"] = "Ada Park", ["c2"] = "Lin Tran" };
var time = new DateTimeOffset(2026, 1, 5, 9, 0, 0, TimeSpan.Zero);
Func<DateTimeOffset> clock = () => time = time.AddSeconds(1);

int lookups = 0;
(Enriched? Msg, string? Error) Enrich(Order o)
{
    lookups++;
    if (!directory.TryGetValue(o.Customer, out var name))
    {
        return (null, $"customer {o.Customer} not found");
    }
    var state = zipPrefix.GetValueOrDefault(o.Zip[..3], "??");
    return (new Enriched(o, name, state, clock()), null);
}

var incoming = new[] { new Order("o1", "c1", "98101"), new Order("o2", "c2", "10001"),
                       new Order("o3", "c1", "60601"), new Order("o4", "c9", "98101") };
int enriched = 0, invalid = 0;
foreach (var o in incoming)
{
    var (m, err) = Enrich(o);
    if (m is null)
    {
        invalid++;
        Console.WriteLine($"invalid: {o.Id} {err}");
        continue;
    }
    enriched++;
    Console.WriteLine($"{o.Id} {o.Customer} {m.Name}, {o.Zip} {m.State}, received {m.Received:HH:mm:ss}");
}
Console.WriteLine($"lookups: {lookups}, enriched: {enriched}, invalid: {invalid}");

record Order(string Id, string Customer, string Zip);
record Enriched(Order Order, string Name, string State, DateTimeOffset Received);
```

```output
o1 c1 Ada Park, 98101 WA, received 09:00:01
o2 c2 Lin Tran, 10001 NY, received 09:00:02
o3 c1 Ada Park, 60601 IL, received 09:00:03
invalid: o4 customer c9 not found
lookups: 4, enriched: 3, invalid: 1
```

- Verdict: idiom. Records hold the order and the enriched message, and `TryGetValue` tells a miss from a hit.
- The enricher returns a tuple of a message and an error, so a miss does not throw.
- `GetValueOrDefault` gives `??` for a ZIP prefix that is not in the table.
