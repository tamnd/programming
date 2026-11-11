```csharp run id=query-syntax
var orders = new[] { new Order("pen", 3, 2), new Order("book", 1, 12), new Order("ink", 2, 5), new Order("pen", 4, 2) };

var totals =
    from o in orders
    group o by o.Item into g
    let total = g.Sum(o => o.Qty * o.Price)
    orderby total descending
    select $"{g.Key}: {total}";

foreach (var line in totals)
{
    Console.WriteLine(line);
}

record Order(string Item, int Qty, int Price);
```

```output for=query-syntax
pen: 14
book: 12
ink: 10
```

- Verdict: native. LINQ query syntax reads like SQL, and the compiler turns it into calls to methods such as `GroupBy`, `OrderByDescending` and `Select`. Microsoft's guide says that with LINQ "you write declarative code that describes what you want to retrieve."
- A LINQ provider such as Entity Framework can translate the same query into SQL for a database.
