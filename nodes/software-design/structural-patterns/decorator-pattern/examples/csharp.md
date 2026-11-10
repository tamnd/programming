```csharp run id=repository
IRepository repo = new Logged(new Cached(new Store()));
foreach (var key in new[] { "a", "a", "b" })
{
    Console.WriteLine(repo.Get(key));
}
Console.WriteLine($"name: {repo.Name()}");

interface IRepository
{
    string Get(string key);
    string Name();
}

sealed class Store : IRepository
{
    public string Get(string key)
    {
        Console.WriteLine($"store: load {key}");
        return $"value-{key}";
    }

    public string Name() => "store";
}

sealed class Logged(IRepository inner) : IRepository
{
    public string Get(string key)
    {
        Console.WriteLine($"log: get {key}");
        return inner.Get(key);
    }

    public string Name() => inner.Name();
}

sealed class Cached(IRepository inner) : IRepository
{
    private readonly Dictionary<string, string> cache = new();

    public string Get(string key)
    {
        if (!cache.TryGetValue(key, out var value))
        {
            value = inner.Get(key);
            cache[key] = value;
        }
        return value;
    }

    public string Name() => inner.Name();
}
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. The decorators implement `IRepository` and take the inner object through a primary constructor parameter.
- `System.IO.BufferedStream` has this shape. It wraps another `Stream` and adds a buffer.
- With a dependency injection container, the chain is often registered once, so that a class that asks for `IRepository` gets the outer decorator.
