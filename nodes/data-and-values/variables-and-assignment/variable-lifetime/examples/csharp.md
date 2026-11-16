```csharp run id=using-declaration
using System;

{
    using var r = new Resource("a");
    Console.WriteLine("in block");
}
Console.WriteLine("after block");

sealed class Resource : IDisposable
{
    private readonly string name;

    public Resource(string name)
    {
        this.name = name;
    }

    public void Dispose() => Console.WriteLine($"dispose {name}");
}
```

```output for=using-declaration
in block
dispose a
after block
```

- Verdict: native. A local variable lives for its block, and a field for its object, which the garbage collector frees later. For a resource, a `using` declaration calls `Dispose` when the variable goes out of scope, so the program knows when the file or the connection ends.
