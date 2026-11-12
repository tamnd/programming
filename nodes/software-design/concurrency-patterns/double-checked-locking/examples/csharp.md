```csharp run id=dcl
using System;
using System.Collections.Generic;
using System.Threading;

class Config
{
    public static int Created;
    public Config() => Interlocked.Increment(ref Created);
}

static class Program
{
    private static volatile Config? instance;
    private static readonly object gate = new object();
    private static int lockedCalls;

    static Config Instance()
    {
        var local = instance;
        if (local == null) // first check, no lock
        {
            lock (gate)
            {
                lockedCalls++;
                local = instance;
                if (local == null) // second check, lock held
                {
                    local = new Config();
                    instance = local;
                }
            }
        }
        return local;
    }

    static void Main()
    {
        var start = new Barrier(8);
        var threads = new List<Thread>();
        for (int w = 0; w < 8; w++)
        {
            var t = new Thread(() =>
            {
                start.SignalAndWait();
                for (int i = 0; i < 1000; i++) Instance();
            });
            threads.Add(t);
            t.Start();
        }
        foreach (var t in threads) t.Join();
        Console.WriteLine("instances created: " + Config.Created);
        Console.WriteLine("calls: " + 8 * 1000);
        Console.WriteLine("calls that took the lock: " + (lockedCalls <= 8 ? "8 or fewer" : lockedCalls.ToString()));
    }
}
```

```output
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

- Verdict: idiom. The field is `volatile` and the second check is inside the `lock`. `Lazy<T>` is the library form of the same work.
