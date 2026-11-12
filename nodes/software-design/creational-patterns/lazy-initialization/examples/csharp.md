```csharp run id=lazy
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

sealed class Settings
{
    private readonly Lazy<Dictionary<string, int>> config;
    private int loads;
    public int Loads => Volatile.Read(ref loads);

    public Settings() =>
        // The default mode, ExecutionAndPublication, runs Load on one thread only.
        config = new Lazy<Dictionary<string, int>>(Load);

    private Dictionary<string, int> Load()
    {
        Console.WriteLine("loading config"); // stands for a slow read of a file
        Interlocked.Increment(ref loads);
        return new Dictionary<string, int> { ["timeout"] = 30 };
    }

    public Dictionary<string, int> Config => config.Value;
}

static class Program
{
    static void Main()
    {
        var s = new Settings();
        Console.WriteLine($"settings created, loads = {s.Loads}");
        Console.WriteLine($"timeout = {s.Config["timeout"]}");
        Console.WriteLine($"timeout = {s.Config["timeout"]}");
        Console.WriteLine($"loads = {s.Loads}");

        var shared = new Settings();
        using var start = new ManualResetEventSlim(false);
        var tasks = Enumerable.Range(0, 8)
            .Select(i => Task.Run(() => { start.Wait(); _ = shared.Config; }))
            .ToArray();
        start.Set(); // all eight tasks ask at the same time
        Task.WaitAll(tasks);
        Console.WriteLine($"8 threads: loads = {shared.Loads}");
    }
}
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
loading config
8 threads: loads = 1
```

- Verdict: stdlib. `System.Lazy<T>` wraps a factory and runs it on the first read of `Value`. The `LazyThreadSafetyMode` argument picks one thread, a race where the first result wins, or no thread safety.
- With a factory, `Lazy<T>` caches an exception: if the first call throws, later reads of `Value` throw the same exception.
- `LazyInitializer.EnsureInitialized` gives the same behaviour for a plain field, with no wrapper object.
