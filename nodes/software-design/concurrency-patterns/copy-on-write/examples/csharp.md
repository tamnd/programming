```csharp run id=copy-on-write
using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Threading;

class Program
{
    static ImmutableList<string> listeners = ImmutableList.Create("listener 0");
    static readonly object writeLock = new object(); // writers only

    static void Main()
    {
        var first = Volatile.Read(ref listeners);
        int iterations = 0, errors = 0;
        var threads = new List<Thread>();
        for (int i = 0; i < 4; i++)
        {
            threads.Add(new Thread(() =>
            {
                for (int r = 0; r < 1000; r++)
                {
                    try
                    {
                        foreach (var name in Volatile.Read(ref listeners))
                        {
                            // call each listener
                        }
                    }
                    catch (InvalidOperationException)
                    {
                        Interlocked.Increment(ref errors);
                    }
                    Interlocked.Increment(ref iterations);
                }
            }));
        }
        threads.Add(new Thread(() =>
        {
            for (int n = 1; n <= 100; n++)
            {
                lock (writeLock)
                {
                    Volatile.Write(ref listeners, listeners.Add("listener " + n)); // a new list
                }
            }
        }));
        foreach (var t in threads) t.Start();
        foreach (var t in threads) t.Join();
        Console.WriteLine("iterations: " + iterations);
        Console.WriteLine("iterations that failed: " + errors);
        Console.WriteLine("listeners at the end: " + listeners.Count);
        Console.WriteLine("length of the first snapshot: " + first.Count);
    }
}
```

```output
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

- Verdict: stdlib. `ImmutableList.Add` returns a new list and leaves the old one as it was; the writer publishes the new list with `Volatile.Write` under a lock that only writers take.
