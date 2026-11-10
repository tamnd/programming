```csharp run id=hof
int Twice(Func<int, int> f, int x) => f(f(x));

Func<A, C> Compose<A, B, C>(Func<B, C> f, Func<A, B> g) => x => f(g(x));

string[] words = ["pear", "fig", "banana"];
Console.WriteLine(Twice(n => n * 3, 2));
Console.WriteLine(string.Join(", ", words.Select(w => w.Length)));
Console.WriteLine(string.Join(", ", words.Where(w => w.Length > 3)));
Console.WriteLine(new[] { 1, 2, 3, 4 }.Aggregate(0, (total, n) => total + n));
Console.WriteLine(string.Join(", ", words.OrderBy(w => w.Length)));
var addThenDouble = Compose<int, int, int>(n => n * 2, n => n + 1);
Console.WriteLine(addThenDouble(5));
```

```output for=hof
18
4, 3, 6
pear, banana
10
fig, pear, banana
12
```

- Verdict: native. A delegate is a type for methods with a given signature. The generic delegates `Func<T, TResult>` and `Action<T>` cover the common shapes.
- A lambda expression or a method group converts to a delegate type. A call through the delegate calls the method.
- LINQ names the common operations `Select` (map), `Where` (filter), `Aggregate` (fold) and `OrderBy` (sort by key).
- `Select` and `Where` use deferred execution. The query runs when code enumerates the result, here inside `string.Join`.
