```csharp run id=best-quote
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

// Each supplier replies after some time with a price in cents.
var suppliers = new SortedDictionary<string, (int DelayMs, int Price)>
{
    ["alpha"] = (20, 1250),
    ["beta"] = (80, 1190),
    ["gamma"] = (1000, 1100),
};
Console.WriteLine($"asked {suppliers.Count} suppliers, wait at most 400 ms");

var tasks = suppliers.ToDictionary(s => s.Key, s => Ask(s.Value.DelayMs, s.Value.Price)); // scatter
await Task.WhenAny(Task.WhenAll(tasks.Values), Task.Delay(400));                          // gather, with a timeout

var quotes = tasks.Where(t => t.Value.IsCompletedSuccessfully).ToDictionary(t => t.Key, t => t.Value.Result);
foreach (var name in suppliers.Keys)
{
    Console.WriteLine(quotes.TryGetValue(name, out var price) ? $"reply from {name}: {price}" : $"no reply from {name} in time");
}
var best = quotes.MinBy(q => q.Value);
Console.WriteLine($"best: {best.Key} at {best.Value}");

static async Task<int> Ask(int delayMs, int price)
{
    await Task.Delay(delayMs); // a stand-in for a request on the channel of the supplier
    return price;
}
```

```output
asked 3 suppliers, wait at most 400 ms
reply from alpha: 1250
reply from beta: 1190
no reply from gamma in time
best: beta at 1190
```

- Verdict: idiom. One `Task` for each supplier scatters the requests, and `Task.WhenAny(Task.WhenAll(tasks), Task.Delay(400))` waits for all of them or for the limit.
- `IsCompletedSuccessfully` selects the tasks that replied in time, and `MinBy` finds the best price.
- In a service, a `CancellationToken` with a timeout also tells the slow requests to stop.
