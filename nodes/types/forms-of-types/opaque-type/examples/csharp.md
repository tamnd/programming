```csharp run id=private-nested
using System;
using System.Collections.Generic;

var q = Queues.Create();
q.Enqueue("a");
q.Enqueue("b");
Console.WriteLine($"{q.Dequeue()} {q.Count}");
// var r = new Queues.ListQueue(); // error: ListQueue is private

public interface IQueue
{
    void Enqueue(string item);
    string Dequeue();
    int Count { get; }
}

public static class Queues
{
    public static IQueue Create() => new ListQueue();

    private sealed class ListQueue : IQueue
    {
        private readonly List<string> _items = [];

        public void Enqueue(string item) => _items.Add(item);

        public string Dequeue()
        {
            var first = _items[0];
            _items.RemoveAt(0);
            return first;
        }

        public int Count => _items.Count;
    }
}
```

```output for=private-nested
a 1
```

- Verdict: idiom. `ListQueue` is a `private` nested class, so only `Queues` can name it or build it. Code outside gets an `IQueue` from `Create` and sees only the members of the interface. An `internal` class gives the same hiding to code outside the assembly.
