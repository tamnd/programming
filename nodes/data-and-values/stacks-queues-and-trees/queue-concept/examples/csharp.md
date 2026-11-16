```csharp run id=queue
var queue = new Queue<string>();
foreach (var item in new[] { "a", "b", "c" })
{
    queue.Enqueue(item);
}
Console.WriteLine($"{string.Join(",", queue)} front {queue.Peek()}");

Console.WriteLine($"{queue.Dequeue()} next {queue.Peek()} size {queue.Count}");
queue.Dequeue();
queue.Dequeue();
Console.WriteLine($"TryDequeue on empty: {(queue.TryDequeue(out var item2) ? item2 : "no")}");
try
{
    queue.Dequeue();
}
catch (InvalidOperationException)
{
    Console.WriteLine("Dequeue on empty: InvalidOperationException");
}
```

```output for=queue
a,b,c front a
a next b size 2
TryDequeue on empty: no
Dequeue on empty: InvalidOperationException
```

- Verdict: stdlib. `Queue<T>` has `Enqueue`, `Dequeue` and `Peek`, and it enumerates from the front.
- `Dequeue` on an empty queue throws `InvalidOperationException`. `TryDequeue` returns `false` instead.
