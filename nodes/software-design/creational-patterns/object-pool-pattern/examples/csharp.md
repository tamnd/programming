```csharp run id=pool
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

sealed class Buffer
{
    public readonly int Id; // a real buffer would be costly to create
    public readonly StringBuilder Text = new();
    public Buffer(int id) => Id = id;
}

sealed class Pool<T> where T : class
{
    private readonly Stack<T> idle = new();
    private readonly Func<int, T> create;
    private readonly Action<T> reset;
    private readonly int maxIdle;
    private readonly object gate = new();
    public int Created { get; private set; }

    public Pool(Func<int, T> create, Action<T> reset, int maxIdle) =>
        (this.create, this.reset, this.maxIdle) = (create, reset, maxIdle);

    public T Acquire()
    {
        lock (gate)
        {
            if (idle.Count > 0) return idle.Pop();
            Created++;
            return create(Created);
        }
    }

    public void Release(T item)
    {
        reset(item); // clear the state before the next user sees it
        lock (gate)
        {
            if (idle.Count < maxIdle) idle.Push(item);
        }
    }

    public Lease Rent() => new Lease(this, Acquire());

    public int Idle { get { lock (gate) return idle.Count; } }

    public readonly struct Lease : IDisposable
    {
        private readonly Pool<T> pool;
        public T Item { get; }
        public Lease(Pool<T> pool, T item) => (this.pool, Item) = (pool, item);
        public void Dispose() => pool.Release(Item); // called at the end of a using block
    }
}

static class Program
{
    static void Main()
    {
        var pool = new Pool<Buffer>(id => new Buffer(id), b => b.Text.Clear(), 2);
        for (int i = 1; i <= 3; i++)
        {
            using var lease = pool.Rent();
            lease.Item.Text.Append($"request {i}");
            Console.WriteLine($"request {i} -> buffer #{lease.Item.Id}: {lease.Item.Text}");
        }

        var held = new List<Buffer> { pool.Acquire(), pool.Acquire(), pool.Acquire() };
        Console.WriteLine("3 at once -> buffers " + string.Join(" ", held.Select(b => b.Id).Order().Select(id => $"#{id}")));
        foreach (var b in held) pool.Release(b);
        Console.WriteLine($"idle after release: {pool.Idle}");
        Console.WriteLine($"created: {pool.Created}");
    }
}
```

```output
request 1 -> buffer #1: request 1
request 2 -> buffer #1: request 2
request 3 -> buffer #1: request 3
3 at once -> buffers #1 #2 #3
idle after release: 2
created: 3
```

- Verdict: stdlib. .NET has `System.Buffers.ArrayPool<T>` for arrays, and the package `Microsoft.Extensions.ObjectPool` for other objects. The example writes its own pool to show the parts.
- A lease that implements `IDisposable` lets a `using` statement return the item at the end of the scope.
- ADO.NET pools database connections behind `SqlConnection.Open` and `Close`, so most code uses a pool without seeing it.
