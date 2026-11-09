```csharp run id=cycle
using System.Runtime.CompilerServices;

WeakReference probe = MakeCycle();
GC.Collect();
Console.WriteLine($"collected after gc: {!probe.IsAlive}");

var item = new Item();
Console.Write($"generation: {GC.GetGeneration(item)}");
GC.Collect();
Console.Write($" {GC.GetGeneration(item)}");
GC.Collect();
Console.WriteLine($" {GC.GetGeneration(item)}");

// NoInlining keeps a and b in a frame that is gone before GC.Collect runs.
[MethodImpl(MethodImplOptions.NoInlining)]
static WeakReference MakeCycle()
{
    var a = new Item();
    var b = new Item { Peer = a };
    a.Peer = b;
    var probe = new WeakReference(a);
    Console.WriteLine($"alive while referenced: {probe.IsAlive}");
    return probe;
}

class Item
{
    public Item? Peer;
}
```

```output
alive while referenced: True
collected after gc: True
generation: 0 1 2
```

- Verdict: native. The .NET collector is generational. A collection marks the live objects and compacts them, and the survivors of a collection move up one generation, from 0 to 1 and from 1 to 2.
- The last line shows the promotion of one object that stays alive through two calls of `GC.Collect()`.
- `[MethodImpl(MethodImplOptions.NoInlining)]` keeps `MakeCycle` a real call, so its locals `a` and `b` are gone before `GC.Collect()` runs.
- Release files and sockets with `using`, not with a finalizer.
