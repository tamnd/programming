```csharp run id=stop-a-thread
using System.Diagnostics;

using var cts = new CancellationTokenSource();
CancellationToken token = cts.Token;

var worker = new Thread(() =>
{
    while (!token.IsCancellationRequested)
    {
        // Waits up to 10 s, but wakes at once when cancellation is requested.
        token.WaitHandle.WaitOne(TimeSpan.FromSeconds(10));
    }
    Console.WriteLine("worker: cancellation requested, cleaning up");
});
worker.Start();
Thread.Sleep(100);

var watch = Stopwatch.StartNew();
cts.Cancel();
worker.Join();
Console.WriteLine($"worker stopped in under 1 s: {watch.Elapsed < TimeSpan.FromSeconds(1)}");
Console.WriteLine($"worker alive after join: {worker.IsAlive}");
```

```output for=stop-a-thread
worker: cancellation requested, cleaning up
worker stopped in under 1 s: True
worker alive after join: False
```

- Verdict: stdlib. .NET has "a unified model for cooperative cancellation" that uses a cancellation token. The code that owns the `CancellationTokenSource` calls `Cancel`, and the worker checks `IsCancellationRequested`.
- The worker waits on `token.WaitHandle`, so the wait ends when cancellation is requested. The documentation lists polling, callbacks and wait handles as the ways to listen.
- `using var cts` disposes the token source at the end. The documentation says to call `Dispose` "when you have finished using the cancellation token source".
- `Thread.Abort` is not an alternative. It "throws a PlatformNotSupportedException at run time on .NET 5 and later and .NET Core".
