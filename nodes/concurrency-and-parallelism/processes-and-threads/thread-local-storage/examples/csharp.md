```csharp run id=per-thread-values
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

static class Program
{
    // Each thread gets its own copy of a [ThreadStatic] field, which starts at 0.
    [ThreadStatic] static int counter;

    static readonly ThreadLocal<string?> User = new();

    static int Bump(int times)
    {
        for (int i = 0; i < times; i++) counter++;
        return counter;
    }

    static void Main()
    {
        var results = new ConcurrentDictionary<string, int>();
        var t1 = new Thread(() => results["thread 1"] = Bump(1));
        var t2 = new Thread(() => results["thread 2"] = Bump(2));
        t1.Start();
        t2.Start();
        t1.Join();
        t2.Join();
        results["main"] = Bump(3);
        foreach (var name in new[] { "main", "thread 1", "thread 2" })
            Console.WriteLine($"{name}: counter = {results[name]}");

        // A worker thread that runs one task after another, as a pool thread does.
        var output = new List<string>();
        Action[] tasks =
        [
            () => User.Value = "alice",
            () => output.Add($"a later task on the same thread sees user: {User.Value ?? "null"}"),
            () => User.Value = null,
            () => output.Add($"after a task removes it, the next task sees user: {User.Value ?? "null"}"),
        ];
        var worker = new Thread(() =>
        {
            foreach (var task in tasks) task();
        });
        worker.Start();
        worker.Join();
        output.ForEach(Console.WriteLine);
    }
}
```

```output for=per-thread-values
main: counter = 3
thread 1: counter = 1
thread 2: counter = 2
a later task on the same thread sees user: alice
after a task removes it, the next task sees user: null
```

- Verdict: stdlib. A static field with `[ThreadStatic]` has a separate value in each thread. `ThreadLocal<T>` is a class with the same effect and a factory for the starting value.
- Do not give a `[ThreadStatic]` field an initial value in its declaration. That code runs once, so only one thread gets the value. Here each thread starts at 0, the default.
- The worker thread runs four tasks in order, as a pool thread does. The value that the first task sets stays until a task clears it.
- For values that must follow `await` from thread to thread, .NET has `AsyncLocal<T>`.
