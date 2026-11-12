```csharp run id=around
using System;
using System.Collections.Generic;

static T WithConnection<T>(string name, Func<string, T> block)
{
    Console.WriteLine($"open {name}");
    try
    {
        return block(name); // the caller's work
    }
    finally
    {
        Console.WriteLine($"close {name}"); // runs even if block throws
    }
}

int result = WithConnection("db", conn =>
{
    var rows = new List<string> { "a", "b", "c" };
    Console.WriteLine($"query: {rows.Count} rows");
    return rows.Count;
});
Console.WriteLine($"result: {result}");
```

```output for=around
open db
query: 3 rows
close db
result: 3
```

- Verdict: native. A method takes the block as a `Func` delegate. For an `IDisposable` resource, the `using` statement does the same work.
