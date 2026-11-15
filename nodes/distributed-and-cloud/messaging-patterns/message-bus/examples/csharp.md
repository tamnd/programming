```csharp run id=add-a-program
var bus = new Bus();
bus.Subscribe("OrderPlaced", m => Console.WriteLine($"billing: charge {m.AmountCents} cents for order {m.Order}"));
bus.Subscribe("OrderPlaced", m => Console.WriteLine($"warehouse: SHIP|{m.Order}"));
ShopAdapter(bus, "order 42 total 19.99");

Console.WriteLine("analytics joins the bus");
int seen = 0;
bus.Subscribe("OrderPlaced", m =>
{
    seen++;
    Console.WriteLine($"analytics: order {m.Order} counted, total orders {seen}");
});
ShopAdapter(bus, "order 43 total 5.00");

// The adapter of the shop reads "order 42 total 19.99".
static void ShopAdapter(Bus bus, string line)
{
    Console.WriteLine($"shop sends: {line}");
    var f = line.Split(' ');
    var total = f[3].Split('.');
    int cents = int.Parse(total[0]) * 100 + int.Parse(total[1]);
    bus.Publish(new Message("OrderPlaced", int.Parse(f[1]), cents));
}

// The canonical message of the bus.
record Message(string Type, int Order, int AmountCents);

sealed class Bus
{
    private readonly Dictionary<string, List<Action<Message>>> subscribers = new();

    public void Subscribe(string type, Action<Message> handler)
    {
        if (!subscribers.TryGetValue(type, out var list))
        {
            subscribers[type] = list = new List<Action<Message>>();
        }
        list.Add(handler);
    }

    public void Publish(Message m)
    {
        Console.WriteLine($"bus: {m.Type} order={m.Order} amount_cents={m.AmountCents}");
        foreach (var h in subscribers.GetValueOrDefault(m.Type, new List<Action<Message>>()))
        {
            h(m);
        }
    }
}
```

```output
shop sends: order 42 total 19.99
bus: OrderPlaced order=42 amount_cents=1999
billing: charge 1999 cents for order 42
warehouse: SHIP|42
analytics joins the bus
shop sends: order 43 total 5.00
bus: OrderPlaced order=43 amount_cents=500
billing: charge 500 cents for order 43
warehouse: SHIP|43
analytics: order 43 counted, total orders 1
```

- Verdict: idiom. .NET has no message bus in the base library. A `Dictionary` from a message type to a list of `Action<Message>` is the bus, and the record `Message` is the common data model.
- The lambda of analytics changes the local variable `seen`; C# lambdas can change the variables that they capture.
- Libraries such as MassTransit give a bus over a broker, with `Publish` for events and `Send` for commands.
