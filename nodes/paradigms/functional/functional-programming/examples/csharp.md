```csharp run id=linq
var orders = new[] { new Order("pen", 3, 2), new Order("book", 1, 12), new Order("ink", 2, 5) };

var total = orders.Where(o => o.Qty > 1).Select(o => o.Qty * o.Price).Sum();
Console.WriteLine(total);

Func<string, string> trim = s => s.Trim();
Func<string, string> lower = s => s.ToLowerInvariant();
Func<string, string> slug = s => lower(trim(s)).Replace(' ', '-');
Console.WriteLine(slug("  Pure Functions  "));

var cheaper = orders[1] with { Price = 10 };
Console.WriteLine($"{cheaper} {orders[1]}");
Console.WriteLine(orders.Aggregate(0, (acc, o) => acc + o.Qty));

record Order(string Item, int Qty, int Price);
```

```output for=linq
16
pure-functions
Order { Item = book, Qty = 1, Price = 10 } Order { Item = book, Qty = 1, Price = 12 }
6
```

- Verdict: native. C# has lambdas, the delegate types `Func` and `Action`, LINQ, records with `with` expressions, and pattern matching. Microsoft's guide says that new constructs "such as lambda expressions and type inference make functional programming much easier".
- `Aggregate` is the name of fold in LINQ.
