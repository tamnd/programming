```csharp run id=counter
var counterLock = new Lock();
int counter = 0;

var workers = new Task[4];
for (int i = 0; i < workers.Length; i++)
{
    workers[i] = Task.Run(() =>
    {
        for (int k = 0; k < 10_000; k++)
        {
            lock (counterLock)
            {
                counter++;
            }
        }
    });
}
Task.WaitAll(workers);
Console.WriteLine($"total: {counter}");

var settings = new ReaderWriterLockSlim();
settings.EnterReadLock();
Console.WriteLine($"readers: {settings.CurrentReadCount}");
settings.ExitReadLock();
```

```output
total: 40000
readers: 1
```

- Verdict: native. The `lock` statement takes a lock, runs the block and releases the lock in a `finally`. With a `System.Threading.Lock`, it calls `EnterScope`. With any other object, it uses `Monitor.Enter` and `Monitor.Exit`.
- The thread that holds a lock can take it again, so `lock` is reentrant.
- Lock a private object that only this code can see. Do not lock `this`, a `Type` or a string, because other code can lock the same object.
- The body of a `lock` statement cannot contain `await`. `ReaderWriterLockSlim` is the reader-writer lock. It does not allow recursion by default, and it has an upgradeable read mode.
