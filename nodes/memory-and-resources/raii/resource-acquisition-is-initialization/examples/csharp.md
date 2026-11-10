```csharp run id=using
Copy(fail: false);
try
{
    Copy(fail: true);
}
catch (IOException error)
{
    Console.WriteLine($"caught: {error.Message}");
}

using (var report = new Resource("report"))
{
    Console.WriteLine($"write {report.Name}");
}
Console.WriteLine("after using statement");

static void Copy(bool fail)
{
    using var source = new Resource("source");
    using var target = new Resource("target");
    if (fail)
    {
        throw new IOException("disk full");
    }
    Console.WriteLine($"copy {source.Name} -> {target.Name}");
}

sealed class Resource : IDisposable
{
    public string Name { get; }

    public Resource(string name)
    {
        Name = name;
        Console.WriteLine($"open {name}");
    }

    public void Dispose() => Console.WriteLine($"close {Name}");
}
```

```output
open source
open target
copy source -> target
close target
close source
open source
open target
close target
close source
caught: disk full
open report
write report
close report
after using statement
```

- Verdict: native. A `using` declaration disposes its variable at the end of the enclosing scope, here the end of `Copy`.
- Two `using var` declarations dispose in the reverse order of declaration: `target`, then `source`. The exception reaches the `catch` after both close.
- The `using` statement with a block disposes at the end of that block. The output shows `close report` before `after using statement`.
- `Dispose` comes from `IDisposable`. The class is `sealed`, so it needs no virtual `Dispose(bool)` method for subclasses.
