```csharp run id=three-layers
using System;
using System.Collections.Generic;

record Order(string Customer, string Item, int Quantity, int Cents);

class Tables
{
    public Dictionary<string, int> Stock = new() { ["widget"] = 10 };
    public List<Order> Orders = new();

    public Tables Copy() => new() { Stock = new(Stock), Orders = new(Orders) }; // the orders are immutable records
}

// Data source layer: the stored data, and a transaction that keeps all or none of a change.
class Store
{
    public Tables Tables = new();

    public bool Transaction(Action<Tables> change)
    {
        var saved = Tables.Copy();
        try
        {
            change(Tables);
            return true;
        }
        catch (InvalidOperationException)
        {
            Tables = saved; // roll back
            return false;
        }
    }
}

class Program
{
    // Domain layer: the business rules, with a special deal for one customer.
    const int PriceCents = 1000;

    static int Price(string customer, int quantity)
    {
        int total = PriceCents * quantity;
        if (customer == "acme" && quantity >= 3)
            total = total * 90 / 100; // a deal that sales made with one customer
        return total;
    }

    static bool PlaceOrder(Store store, string customer, string item, int quantity) =>
        store.Transaction(tables =>
        {
            tables.Orders.Add(new Order(customer, item, quantity, Price(customer, quantity)));
            if (tables.Stock[item] < quantity) throw new InvalidOperationException("not enough stock");
            tables.Stock[item] -= quantity;
        });

    static string Money(int cents) => $"{cents / 100}.{cents % 100:D2}";

    // Presentation layer: two screens show the same data in different ways.
    static void OrderScreen(Store store)
    {
        foreach (var o in store.Tables.Orders)
            Console.WriteLine($"order screen: {o.Customer} bought {o.Quantity} x {o.Item} for {Money(o.Cents)}");
    }

    static void StockScreen(Store store)
    {
        foreach (var (item, count) in store.Tables.Stock)
            Console.WriteLine($"stock screen: {item}: {count} in stock, orders: {store.Tables.Orders.Count}");
    }

    static void Main()
    {
        var store = new Store();
        Console.WriteLine("acme orders 3: " + (PlaceOrder(store, "acme", "widget", 3) ? "kept" : "rolled back"));
        Console.WriteLine("bolt orders 9: " + (PlaceOrder(store, "bolt", "widget", 9) ? "kept" : "rolled back"));
        OrderScreen(store);
        StockScreen(store);
    }
}
```

```output
acme orders 3: kept
bolt orders 9: rolled back
order screen: acme bought 3 x widget for 27.00
stock screen: widget: 7 in stock, orders: 1
```

- Verdict: library. ASP.NET Core gives the screens and Entity Framework Core gives the data. The example uses records for the orders.
