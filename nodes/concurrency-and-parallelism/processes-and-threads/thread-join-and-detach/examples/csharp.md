```csharp run id=background
using System;
using System.Threading;

var worker = new Thread(() => Console.WriteLine("worker ran"));
worker.Start();
worker.Join();
Console.WriteLine($"after Join, alive: {worker.IsAlive}");

var sleeper = new Thread(() => Thread.Sleep(Timeout.Infinite)) { IsBackground = true };
sleeper.Start();
bool ended = sleeper.Join(50);
Console.WriteLine($"background thread ended within 50 ms: {ended}");
Console.WriteLine("main returns; the runtime does not wait for the background thread");
```

```output for=background
worker ran
after Join, alive: False
background thread ended within 50 ms: False
main returns; the runtime does not wait for the background thread
```

- Verdict: stdlib. `Thread.Join()` waits without a limit, and `Join(int)` waits for a number of milliseconds and returns `true` if the thread has ended.
- .NET has no detach for a `Thread`. A background thread, with `IsBackground = true`, does not keep the process running, so it acts like a detached thread at exit.
- The sleeper thread never ends by itself. The process exits when the top-level statements return, because only a background thread is left.
- Threads from the thread pool are background threads. A `Task` is waited for with `Wait` or `await`, not with `Join`.
