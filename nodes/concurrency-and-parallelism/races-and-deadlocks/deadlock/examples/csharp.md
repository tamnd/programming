```csharp run id=try-enter
using System;
using System.Threading;

var a = new object();
var b = new object();
using var bothHold = new Barrier(2);
using var bothTried = new Barrier(2);
var got = new bool[2];

Thread Worker(int index, object first, object second) => new Thread(() =>
{
    lock (first)
    {
        bothHold.SignalAndWait();
        got[index] = Monitor.TryEnter(second, TimeSpan.FromMilliseconds(200));
        if (got[index]) Monitor.Exit(second);
        bothTried.SignalAndWait();
    }
});

var t1 = Worker(0, a, b);
var t2 = Worker(1, b, a);
t1.Start();
t2.Start();
t1.Join();
t2.Join();
Console.WriteLine($"t1 got its second lock: {got[0]}");
Console.WriteLine($"t2 got its second lock: {got[1]}");
```

```output for=try-enter
t1 got its second lock: False
t2 got its second lock: False
```

- Verdict: native. The `lock` statement takes the monitor of an object and releases it at the end of the block. It has no timeout, so a cycle of `lock` statements waits forever.
- `Monitor.TryEnter` with a `TimeSpan` returns `false` when the time runs out. It is the try-lock for the back off method.
- `lock` is reentrant: the thread that holds the lock can take it again.
- The compiler rejects `await` inside a `lock` body.
