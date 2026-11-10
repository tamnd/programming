```csharp run id=request
var orders = new Request { Url = "https://example.com/orders" };
Show(orders);
Show(orders with { Method = "POST", Headers = ["Accept: application/json"], Timeout = 5, Retries = 2 });
try
{
    _ = new Request { Url = "https://example.com/orders", Timeout = 0 };
}
catch (ArgumentOutOfRangeException error)
{
    Console.WriteLine($"error: {error.ParamName} must be positive");
}

static void Show(Request r) =>
    Console.WriteLine($"{r.Method} {r.Url} timeout={r.Timeout} retries={r.Retries} headers=[{string.Join(", ", r.Headers)}]");

record Request
{
    private readonly int _timeout = 30;

    public required string Url { get; init; }
    public string Method { get; init; } = "GET";
    public IReadOnlyList<string> Headers { get; init; } = [];
    public int Retries { get; init; }

    public int Timeout
    {
        get => _timeout;
        init => _timeout = value > 0 ? value : throw new ArgumentOutOfRangeException("timeout");
    }
}
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
```

- Verdict: native. An object initializer sets named `init` properties after the constructor, and each property has its default. Named and optional arguments of a constructor or a method are a second built-in form.
- `required` makes the compiler reject an initializer that leaves out `Url`. A Java builder checks the same thing at run time, or takes the part in its constructor.
- An `init` accessor can run only during initialization, so the record is immutable afterwards. The `init` accessor of `Timeout` checks its value.
- A `with` expression copies a record and changes the named properties. It takes the place of a builder that starts from an existing product.
