```csharp run id=force-query
var prices = new List<int> { 10, 20, 30 };

// Deferred: the query reads the list each time the program enumerates it.
IEnumerable<int> deferred = prices.Select(price => price * 2);
// Immediate: ToList runs the query now and stores the result.
List<int> forced = prices.Select(price => price * 2).ToList();

prices.Add(40);
Console.WriteLine(string.Join(", ", deferred));
Console.WriteLine(string.Join(", ", forced));
```

```output for=force-query
20, 40, 60, 80
20, 40, 60
```

- `Select`, `Where` and the other operators that return `IEnumerable<T>` use deferred execution. The query runs again each time the program enumerates it.
- `ToList` and `ToArray` force the query once and keep the result.
- A deferred query over a resource, such as a database context or a file, must be forced before the program disposes of the resource.
