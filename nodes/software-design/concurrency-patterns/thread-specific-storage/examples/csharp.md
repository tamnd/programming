```csharp run id=tss
using System;
using System.Threading;

class Program
{
    [ThreadStatic] private static int lastError; // one field, one value per thread

    static int FakeCall(int code)
    {
        lastError = code; // the call sets its thread's error
        Thread.Yield(); // let another thread run between the set and the test
        return -1;
    }

    static void Main()
    {
        int wrongReads = 0;
        int[] seen = new int[4];
        var threads = new Thread[4];
        for (int i = 0; i < 4; i++)
        {
            int index = i;
            threads[i] = new Thread(() =>
            {
                int code = index + 1;
                for (int n = 0; n < 1000; n++)
                {
                    if (FakeCall(code) == -1 && lastError != code) Interlocked.Increment(ref wrongReads);
                }
                seen[index] = lastError;
            });
            threads[i].Start();
        }
        foreach (var t in threads) t.Join();
        Console.WriteLine("threads: " + threads.Length);
        Console.WriteLine("calls per thread: " + 1000);
        Console.WriteLine("reads that saw another thread's error: " + wrongReads);
        Console.WriteLine("error code seen by each thread: [" + string.Join(", ", seen) + "]");
    }
}
```

```output
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

- Verdict: native. The `[ThreadStatic]` attribute gives each thread its own copy of the static field.
