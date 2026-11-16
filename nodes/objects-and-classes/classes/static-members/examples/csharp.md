```csharp run id=static-class-and-members
using System;

var a = new Order(100);
var b = Order.FromText("50");
Console.WriteLine($"{a.Id} {b.Id} {Order.Created}");
Console.WriteLine($"{a.Total()} {Money.Round(2.345)}");

class Order
{
    public const double TaxRate = 0.2; // a const is static too
    public static int Created { get; private set; } // a static property

    public int Id { get; }
    private readonly int amount;

    public Order(int amount)
    {
        Created++;
        Id = Created;
        this.amount = amount;
    }

    public static Order FromText(string text) => new(int.Parse(text));

    public double Total() => amount * (1 + TaxRate);
}

static class Money // a static class: no instances, only static members
{
    public static double Round(double value) => Math.Round(value, 2);
}
```

```output for=static-class-and-members
1 2 2
120 2.35
```

- Verdict: native. `static` fields, properties and methods belong to the type, and a `const` field is static without the keyword.
- A `static class` such as `Money` cannot have instances: `new Money()` is a compile error. It holds helper functions, like `System.Math`.
- A static member is reached only through the type name: `a.Created` is a compile error in C#, unlike in Java.
