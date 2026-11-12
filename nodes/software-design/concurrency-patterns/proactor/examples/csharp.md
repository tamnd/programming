```csharp run id=proactor
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

static class Program
{
    static void Main()
    {
        var files = new Dictionary<string, string> { ["a.txt"] = "hello", ["b.txt"] = "abc" };
        var operations = new BlockingCollection<(string Name, Action<string, int> Handler)>();
        var completions = new BlockingCollection<(Action<string, int> Handler, string Name, int Size)>();

        var processor = new Thread(() =>
        {
            foreach (var (name, handler) in operations.GetConsumingEnumerable())
            {
                completions.Add((handler, name, files[name].Length));
            }
        });
        processor.Start();

        Action<string, int> onRead = (name, size) => Console.WriteLine($"{name} done: {size} bytes");
        foreach (var name in new[] { "a.txt", "b.txt" })
        {
            Console.WriteLine($"start read {name}");
            operations.Add((name, onRead));
        }
        operations.CompleteAdding();
        for (int i = 0; i < 2; i++)
        {
            var (handler, name, size) = completions.Take(); // the Completion Dispatcher
            handler(name, size);
        }
        processor.Join();
    }
}
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: stdlib. `BlockingCollection` gives the two queues, and the handler is an `Action<string, int>` delegate.
