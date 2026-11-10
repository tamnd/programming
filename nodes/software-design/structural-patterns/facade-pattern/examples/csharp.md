```csharp run id=shop
var shop = new Shop(new Dictionary<string, int> { ["book"] = 2, ["lamp"] = 0 });
Console.WriteLine(shop.PlaceOrder("book", "visa", 12.50m));
Console.WriteLine(shop.PlaceOrder("book", "expired", 12.50m));
Console.WriteLine(shop.PlaceOrder("lamp", "visa", 30.00m));
Console.WriteLine($"stock of book: {shop.Stock("book")}");

sealed class Inventory(Dictionary<string, int> stock)
{
    public bool Reserve(string item)
    {
        if (stock.GetValueOrDefault(item) == 0) return false;
        stock[item]--;
        return true;
    }

    public void Release(string item) => stock[item] = stock.GetValueOrDefault(item) + 1;

    public int Count(string item) => stock.GetValueOrDefault(item);
}

sealed class Payments
{
    public bool Charge(string card, decimal amount) => card != "expired";
}

sealed class Shipping
{
    int _parcels;

    public int Schedule(string item) => ++_parcels;
}

// The facade: one class with the two operations that clients need.
sealed class Shop(Dictionary<string, int> stock)
{
    readonly Inventory _inventory = new(new Dictionary<string, int>(stock));
    readonly Payments _payments = new();
    readonly Shipping _shipping = new();

    public string PlaceOrder(string item, string card, decimal price)
    {
        if (!_inventory.Reserve(item)) return $"failed: {item} is out of stock";
        if (!_payments.Charge(card, price))
        {
            _inventory.Release(item);
            return $"failed: card declined, {item} back in stock";
        }
        return $"ok: {item}, paid {price.ToString("F2", System.Globalization.CultureInfo.InvariantCulture)}, parcel {_shipping.Schedule(item)}";
    }

    public int Stock(string item) => _inventory.Count(item);
}
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: idiom. `Shop` is the facade. It makes its subsystem objects and keeps them in private fields.
- The price is a `decimal`, which holds `12.50` exactly. Money in a `double` can show rounding errors.
- In a library, the subsystem classes would be `internal`, so code in other assemblies sees only `Shop`.
- An ASP.NET Core application would register the subsystem classes with the dependency injection container, and the container would pass them to the constructor of `Shop`.
