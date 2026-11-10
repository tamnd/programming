```csharp run id=report
#:property PublishAot=false
using System.Reflection;

var lazy = new Lazy<IReport>(() => new DiskReport());
IReport report = new LazyReport(lazy);
Console.WriteLine("proxy made");
Console.WriteLine(report.Render());
Console.WriteLine(report.Render());
foreach (var role in new[] { "guest", "admin" })
{
    Console.WriteLine($"{role}: {Guard.For(report, role).Render()}");
}

public interface IReport
{
    string Render();
}

sealed class DiskReport : IReport
{
    readonly int _orders;

    public DiskReport()
    {
        Console.WriteLine("load: read the report from disk");
        _orders = 42;
    }

    public string Render() => $"report: {_orders} orders";
}

// A virtual proxy: Lazy<T> makes the real object on the first read of Value.
sealed class LazyReport(Lazy<IReport> real) : IReport
{
    public string Render() => real.Value.Render();
}

// A protection proxy made at run time: DispatchProxy sends each call to Invoke.
public class Guard : DispatchProxy
{
    IReport _inner = null!;
    string _role = "";

    public static IReport For(IReport inner, string role)
    {
        var proxy = Create<IReport, Guard>();
        var guard = (Guard)(object)proxy;
        guard._inner = inner;
        guard._role = role;
        return proxy;
    }

    protected override object? Invoke(MethodInfo? method, object?[]? args)
    {
        if (method?.Name == nameof(IReport.Render) && _role != "admin")
        {
            return "access denied";
        }
        return method?.Invoke(_inner, args);
    }
}
```

```output
proxy made
load: read the report from disk
report: 42 orders
report: 42 orders
guest: access denied
admin: report: 42 orders
```

- Verdict: stdlib. `Lazy<T>` holds the factory and makes the value on the first read of `Value`. By default it is thread safe, so two threads that read `Value` at once get the same object.
- `DispatchProxy.Create<TInterface, TProxy>` makes at run time an object that implements the interface and derives from `TProxy`. Each call goes to `Invoke` with the `MethodInfo` of the method.
- `DispatchProxy` generates code at run time. A file-based C# program is compiled for native AOT by default, and native AOT has no code generation at run time, so `Create` throws `PlatformNotSupportedException`. The `#:property PublishAot=false` line turns AOT off for this file.
- `Create` takes no arguments, so `For` sets the fields of the guard after `Create` returns. The cast through `object` reaches the `Guard` part of the generated object.
- The lazy proxy is a class written by hand, because it has one method. A dynamic proxy pays off when an interface has many methods that all need the same treatment.
