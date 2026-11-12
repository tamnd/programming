```csharp run id=order-data
using System;
using System.Collections.Generic;

record OrderData(int OrderId, string Customer); // init-only properties

class Order
{
    private readonly OrderData data; // fixed after construction
    private readonly List<string> items; // changes

    public Order(int orderId, string customer, IEnumerable<string> items)
    {
        data = new OrderData(orderId, customer);
        this.items = new List<string>(items);
    }

    public int OrderId => data.OrderId;
    public string Customer => data.Customer;
    public void Add(string item) => items.Add(item);
    public string Describe() => $"order {OrderId} for {Customer}: {items.Count} items";
}

class Program
{
    static void Main()
    {
        var order = new Order(42, "ann", new[] { "pen", "ink" });
        Console.WriteLine(order.Describe());
        order.Add("paper");
        Console.WriteLine(order.Describe());
    }
}
```

```output
order 42 for ann: 2 items
order 42 for ann: 3 items
```

- Verdict: idiom. The properties of a positional record have `init` accessors, so they can be set only during construction. A `with` expression makes a changed copy and leaves the original as it was.
