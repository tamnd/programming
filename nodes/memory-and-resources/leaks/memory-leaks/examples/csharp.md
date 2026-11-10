```csharp run id=event-handler
using System.Runtime.CompilerServices;

var clock = new Clock(); // lives as long as the program

WeakReference leaked = Subscribe(clock, unsubscribe: false);
WeakReference removed = Subscribe(clock, unsubscribe: true);
GC.Collect();
GC.WaitForPendingFinalizers();
GC.Collect();

Console.WriteLine($"subscriber without -= still alive: {leaked.IsAlive}");
Console.WriteLine($"subscriber with -= collected: {!removed.IsAlive}");
Console.WriteLine($"handlers left on the clock: {clock.HandlerCount}");
GC.KeepAlive(clock);

// NoInlining keeps the subscriber in a frame that is gone before GC.Collect runs.
[MethodImpl(MethodImplOptions.NoInlining)]
static WeakReference Subscribe(Clock clock, bool unsubscribe)
{
    var view = new View();
    clock.Tick += view.OnTick;
    if (unsubscribe)
    {
        clock.Tick -= view.OnTick;
    }
    return new WeakReference(view);
}

class Clock
{
    public event EventHandler? Tick;
    public int HandlerCount => Tick?.GetInvocationList().Length ?? 0;
}

class View
{
    private readonly byte[] buffer = new byte[10_000];
    public void OnTick(object? sender, EventArgs e) => buffer[0]++;
}
```

```output
subscriber without -= still alive: True
subscriber with -= collected: True
handlers left on the clock: 1
```

- Verdict: native. The .NET collector frees objects that no root can reach. An event holds a delegate, and the delegate holds the object of its method, so the publisher keeps each subscriber alive.
- Only the subscriber that ran `-=` is collected. The other one lives as long as `clock` does.
- In a dump, `dotnet-gcdump` or the SOS command `gcroot` shows the path from a root through the event to the subscriber.
