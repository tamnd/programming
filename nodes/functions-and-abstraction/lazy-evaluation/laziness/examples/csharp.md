```csharp run id=deferred
var numbers = new List<int> { 1, 2, 3 };
var doubled = numbers.Select(n => n * 2);
numbers.Add(4);
Console.WriteLine(string.Join(",", doubled));

var config = new Lazy<string>(() =>
{
    Console.WriteLine("reading config");
    return "debug";
});
Console.WriteLine(config.IsValueCreated);
Console.WriteLine(config.Value + " " + config.Value);

static IEnumerable<int> Naturals()
{
    for (int n = 1; ; n++)
    {
        yield return n;
    }
}

Console.WriteLine(string.Join(",", Naturals().Where(n => n % 3 == 0).Take(3)));
```

```output for=deferred
2,4,6,8
False
reading config
debug debug
3,6,9
```

- Verdict: stdlib. Most LINQ operators use deferred execution: the query runs when code enumerates it, and it reads the source at that time. `yield return` makes a lazy sequence.
- `Lazy<T>` computes a value on first access, and it is safe for use by many threads by default.

```csharp wrong id=twice
int calls = 0;
var query = new[] { 1, 2, 3 }.Select(n =>
{
    calls++;
    return n * n;
});
Console.WriteLine(query.Sum());
Console.WriteLine(query.Max());
Console.WriteLine($"selector ran {calls} times");
```

```output for=twice
14
9
selector ran 6 times
```

Each enumeration runs the query again. If the query is slow or has side effects, call `ToList()` one time and use the list.
