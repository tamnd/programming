```csharp run id=checkout
var cart = new List<Item> { new("book", 1500, 2), new("pen", 200, 5) };

Pricing regular = items => items.Sum(i => i.Price * i.Quantity);
Pricing tenPercentOff = items => regular(items) * 90 / 100;
// 50 cents off each unit of a line with 5 or more units.
Pricing bulk = items => items.Sum(i => (i.Quantity >= 5 ? i.Price - 50 : i.Price) * i.Quantity);

var checkout = new Checkout(regular);
foreach (var (name, pricing) in new (string, Pricing)[]
{
    ("regular", regular),
    ("ten percent off", tenPercentOff),
    ("bulk", bulk),
})
{
    checkout.Pricing = pricing;
    Console.WriteLine($"{name}: {checkout.Total(cart)} cents");
}

record Item(string Name, int Price, int Quantity);

delegate int Pricing(IReadOnlyList<Item> items);

class Checkout(Pricing pricing)
{
    public Pricing Pricing { get; set; } = pricing;

    public int Total(IReadOnlyList<Item> items) => Pricing(items);
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. A delegate type is the strategy interface, and a lambda is a strategy. The base library uses the same form in `Comparison<T>`, which `List<T>.Sort` takes.
- `Func<IReadOnlyList<Item>, int>` would also work. A named delegate type such as `Pricing` documents the role and cannot be mixed up with another `Func` of the same shape.
- The class `Checkout` uses a primary constructor. Its parameter initializes the property.
