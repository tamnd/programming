```csharp run id=kinds
using System;
using System.Linq;

abstract class Checkout
{
    public int Total(int[] prices) => Discount(prices.Sum());
    protected abstract int Discount(int amount);
}

class SaleCheckout : Checkout
{
    protected override int Discount(int amount) => amount * 9 / 10;
}

class Till
{
    public Func<int, int> Discount;
    public Till(Func<int, int> discount) { Discount = discount; }
    public int Total(int[] prices) => Discount(prices.Sum());
}

static class Program
{
    static void Main()
    {
        int[] prices = { 40, 60 };
        Console.WriteLine("class pattern: " + new SaleCheckout().Total(prices));
        var till = new Till(amount => amount * 9 / 10);
        Console.WriteLine("object pattern: " + till.Total(prices));
        till.Discount = amount => amount - 20;
        Console.WriteLine("changed at run time: " + till.Total(prices));
    }
}
```

```output
class pattern: 90
object pattern: 90
changed at run time: 80
```

- Verdict: idiom. The strategy is a delegate of type `Func<int, int>`, and a lambda creates it. The class pattern uses an abstract method and `override`.
