```csharp run id=transaction-script
using System;
using System.Collections.Generic;

class Customer
{
    public string Name = "";
    public int Credit;
}

class Order
{
    public int Customer;
    public int Cents;
    public string State = "open";
}

class Program
{
    // A thin wrapper over the stored tables. The scripts call it directly.
    static readonly Dictionary<int, Customer> Customers = new() { [1] = new Customer { Name = "ada", Credit = 5000 } };
    static readonly Dictionary<int, Order> Orders = new();

    static Customer FindCustomer(int customerId) => Customers[customerId];

    static int InsertOrder(int customerId, int cents)
    {
        int orderId = Orders.Count + 1;
        Orders[orderId] = new Order { Customer = customerId, Cents = cents };
        return orderId;
    }

    static string Money(int cents) => $"{cents / 100}.{cents % 100:D2}";

    // One transaction script for each request from the presentation.
    static string PlaceOrder(int customerId, int cents)
    {
        var customer = FindCustomer(customerId);
        if (customer.Credit < cents) return $"refused, credit left {Money(customer.Credit)}";
        customer.Credit -= cents;
        return $"order {InsertOrder(customerId, cents)}";
    }

    static string CancelOrder(int orderId)
    {
        var order = Orders[orderId];
        if (order.State != "open") return "already cancelled";
        order.State = "cancelled";
        FindCustomer(order.Customer).Credit += order.Cents;
        return $"refunded {Money(order.Cents)}";
    }

    static void Main()
    {
        Console.WriteLine("place_order(ada, 30.00): " + PlaceOrder(1, 3000));
        Console.WriteLine("place_order(ada, 30.00): " + PlaceOrder(1, 3000));
        Console.WriteLine("cancel_order(1): " + CancelOrder(1));
        Console.WriteLine("cancel_order(1): " + CancelOrder(1));
        Console.WriteLine("place_order(ada, 30.00): " + PlaceOrder(1, 3000));
        Console.WriteLine("credit of ada: " + Money(FindCustomer(1).Credit));
    }
}
```

```output
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

- Verdict: idiom. Each script is a static method, and the tables are dictionaries of small row classes.
