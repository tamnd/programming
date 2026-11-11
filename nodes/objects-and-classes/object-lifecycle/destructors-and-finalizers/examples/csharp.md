```csharp run id=dispose-pattern
using (var a = new Conn("a"))
{
    a.Use();
}

var b = new Conn("b");
b.Use();
b.Dispose();
b.Dispose();  // a second call does nothing
Console.WriteLine("end");

// The dispose pattern: Dispose for the program, a finalizer as a safety net.
class Conn : IDisposable
{
    private readonly string name;
    private bool disposed;

    public Conn(string name) => this.name = name;

    public void Use() => Console.WriteLine($"using {name}");

    public void Dispose()
    {
        Dispose(disposing: true);
        GC.SuppressFinalize(this);  // the finalizer has nothing left to do
    }

    protected virtual void Dispose(bool disposing)
    {
        if (disposed) return;
        disposed = true;
        // When disposing is false, the finalizer is running: other managed objects
        // may already be finalized, so release only unmanaged resources.
        Console.WriteLine($"release {name} (disposing: {disposing})");
    }

    ~Conn() => Dispose(disposing: false);
}
```

```output for=dispose-pattern
using a
release a (disposing: True)
using b
release b (disposing: True)
end
```

- Verdict: native. A C# class can declare a finalizer with the syntax `~Conn()`, which the garbage collector calls at a time it chooses, on a separate thread. The dispose pattern pairs it with `Dispose`: the program calls `Dispose`, often through `using`, and `GC.SuppressFinalize` tells the collector that the finalizer has nothing left to do. When the finalizer runs, `disposing` is false, and the method must not touch other managed objects, because the collector may have finalized them already. Most classes need no finalizer: a `SafeHandle` wraps an operating system handle and has its own.
