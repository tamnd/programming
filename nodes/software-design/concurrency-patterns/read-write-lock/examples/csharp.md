```csharp run id=read-write-lock
using System;
using System.Collections.Generic;
using System.Threading;

class Program
{
    static readonly ReaderWriterLockSlim rw = new ReaderWriterLockSlim();
    static readonly int[] pair = { 0, 0 }; // the writer keeps the two values equal
    static int inside, reads, mixed, writes, overlap;

    static void RunAll(List<Thread> threads)
    {
        foreach (var t in threads) t.Start();
        foreach (var t in threads) t.Join();
    }

    static void Main()
    {
        // Part 1: two readers hold the read lock at the same time.
        var meeting = new Barrier(2);
        int met = 0;
        var meets = new List<Thread>();
        for (int i = 0; i < 2; i++)
        {
            meets.Add(new Thread(() =>
            {
                rw.EnterReadLock();
                try
                {
                    // Returns true only when both readers are inside.
                    if (meeting.SignalAndWait(TimeSpan.FromSeconds(5))) Interlocked.Increment(ref met);
                }
                finally
                {
                    rw.ExitReadLock();
                }
            }));
        }
        RunAll(meets);

        // Part 2: readers and a writer share the pair.
        var threads = new List<Thread>();
        for (int r = 0; r < 4; r++)
        {
            threads.Add(new Thread(() =>
            {
                for (int n = 0; n < 1000; n++)
                {
                    rw.EnterReadLock();
                    try
                    {
                        Interlocked.Increment(ref inside);
                        int x = pair[0];
                        Thread.Yield(); // give the writer a chance to run here
                        int y = pair[1];
                        Interlocked.Decrement(ref inside);
                        Interlocked.Increment(ref reads);
                        if (x != y) Interlocked.Increment(ref mixed);
                    }
                    finally
                    {
                        rw.ExitReadLock();
                    }
                }
            }));
        }
        threads.Add(new Thread(() =>
        {
            for (int i = 1; i <= 100; i++)
            {
                rw.EnterWriteLock();
                try
                {
                    if (Volatile.Read(ref inside) > 0) overlap++;
                    pair[0] = i;
                    Thread.Yield();
                    pair[1] = i;
                    writes++;
                }
                finally
                {
                    rw.ExitWriteLock();
                }
            }
        }));
        RunAll(threads);

        Console.WriteLine("two readers inside at once: " + (met == 2 ? "True" : "False"));
        Console.WriteLine("reads: " + reads);
        Console.WriteLine("reads that saw a mixed pair: " + mixed);
        Console.WriteLine("writes: " + writes);
        Console.WriteLine("writes that ran with a reader inside: " + overlap);
    }
}
```

```output
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

- Verdict: stdlib. `ReaderWriterLockSlim` in `System.Threading` gives `EnterReadLock` and `EnterWriteLock`.
