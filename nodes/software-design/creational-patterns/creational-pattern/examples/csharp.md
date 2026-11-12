```csharp run id=language-features
using System;
using System.Collections.Generic;

// An object initializer sets properties when the object is created.
var options = new Options { Url = "https://example.com/", Retries = 2 };
Console.WriteLine($"initializer: {options.Url} {options.Method} {options.Retries}");

// A with expression makes a copy of a record with some changes.
var get = new Request("https://example.com/", "GET");
var post = get with { Method = "POST" };
Console.WriteLine($"changed copy: {post}");

// A Func<T> is a factory object.
Func<List<int>> factory = () => new List<int>();
Console.WriteLine($"factory: a new object each time: {!ReferenceEquals(factory(), factory())}");

// Lazy<T> in a static field gives a thread-safe singleton that is made on first use.
Console.WriteLine($"singleton: {ReferenceEquals(Clock.Instance, Clock.Instance)}, created {Clock.Created} time");

sealed class Options
{
    public string Url { get; init; } = "";
    public string Method { get; init; } = "GET";
    public int Retries { get; init; }
}

record Request(string Url, string Method);

sealed class Clock
{
    static readonly Lazy<Clock> instance = new(() => new Clock());
    public static int Created;
    Clock() => Created++;
    public static Clock Instance => instance.Value;
}
```

```output
initializer: https://example.com/ GET 2
changed copy: Request { Url = https://example.com/, Method = POST }
factory: a new object each time: True
singleton: True, created 1 time
```

- Verdict: idiom. Object initializers with `init` properties replace most builders, and a `with` expression copies a record with changes. A delegate such as `Func<T>` is a factory, and `Lazy<T>` gives a lazy singleton.
