```csharp run id=immutable-object
using System;
using System.Collections.Generic;
using System.Threading;

sealed record Settings(int Port, string Tag); // positional record: init-only properties

class Program
{
    static readonly Settings first = new Settings(8000, "port 8000");
    static volatile Settings current = first; // the writer replaces the reference

    static void Main()
    {
        int reads = 0, mixed = 0;
        var threads = new List<Thread>();
        for (int i = 0; i < 4; i++)
        {
            threads.Add(new Thread(() =>
            {
                for (int r = 0; r < 1000; r++)
                {
                    Settings s = current; // one read gives a consistent snapshot
                    Thread.Yield();
                    if (s.Tag != "port " + s.Port) Interlocked.Increment(ref mixed);
                    Interlocked.Increment(ref reads);
                }
            }));
        }
        threads.Add(new Thread(() =>
        {
            for (int port = 8001; port <= 8100; port++)
            {
                current = new Settings(port, "port " + port); // a new object, never a change
                Thread.Yield();
            }
        }));
        foreach (var t in threads) t.Start();
        foreach (var t in threads) t.Join();
        Console.WriteLine("reads: " + reads);
        Console.WriteLine("reads that saw a mixed pair: " + mixed);
        Console.WriteLine("settings published: " + (1 + current.Port - first.Port));
        Console.WriteLine("first settings unchanged: " + (first == new Settings(8000, "port 8000")));
    }
}
```

```output
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

- Verdict: native. A positional `record` has init-only properties, so a `Settings` cannot change after construction, and record equality compares the values.
