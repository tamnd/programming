```csharp run id=payment-receiver
const int Window = 3;
var deliveries = new (string Id, int Order, int Cents)[]
{
    ("pay-1", 17, 1250), ("pay-2", 18, 800), ("pay-1", 17, 1250),
    ("pay-3", 17, 300), ("pay-4", 19, 500), ("pay-5", 20, 100),
    ("pay-2", 18, 800),
};

var receiver = new PaymentReceiver(Window);
foreach (var (id, order, cents) in deliveries)
    Console.WriteLine(receiver.Receive(id, order, cents));

class PaymentReceiver(int window)
{
    readonly Queue<string> order = new();   // ids in the window, oldest first
    readonly HashSet<string> seen = new();
    readonly Dictionary<int, int> totals = new();
    readonly HashSet<string> ever = new();  // only to explain the output

    public string Receive(string id, int orderId, int cents)
    {
        if (seen.Contains(id)) return $"{id}: duplicate, ignored";
        totals[orderId] = totals.GetValueOrDefault(orderId) + cents;
        seen.Add(id);
        order.Enqueue(id);
        if (order.Count > window) seen.Remove(order.Dequeue());
        var note = ever.Add(id) ? "" : $" (applied twice: the id left the window of {window} ids)";
        return $"{id}: order {orderId} +{cents}, total {totals[orderId]}{note}";
    }
}
```

```output
pay-1: order 17 +1250, total 1250
pay-2: order 18 +800, total 800
pay-1: duplicate, ignored
pay-3: order 17 +300, total 1550
pay-4: order 19 +500, total 500
pay-5: order 20 +100, total 100
pay-2: order 18 +800, total 1600 (applied twice: the id left the window of 3 ids)
```

- Verdict: idiom. A `HashSet<string>` gives the lookup, and a `Queue<string>` keeps the ids in the order of arrival for the window.
- `GetValueOrDefault` reads 0 for an order with no payment yet.
- The set `ever` exists only to explain the output; `HashSet.Add` returns `false` for an id that came before.
