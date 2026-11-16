```csharp run id=pq
var tasks = new PriorityQueue<string, int>();
tasks.Enqueue("update docs", 3);
tasks.Enqueue("fix outage", 1);
tasks.Enqueue("review patch", 2);
tasks.Enqueue("plan sprint", 4);

Console.WriteLine("peek: " + tasks.Peek());
while (tasks.TryDequeue(out string? name, out int priority))
{
    Console.WriteLine(priority + " " + name);
}

var maxHeap = new PriorityQueue<int, int>(Comparer<int>.Create((a, b) => b.CompareTo(a)));
foreach (int n in new[] { 3, 1, 2, 4 })
{
    maxHeap.Enqueue(n, n);
}
var order = new List<int>();
while (maxHeap.Count > 0)
{
    order.Add(maxHeap.Dequeue());
}
Console.WriteLine("largest first: " + string.Join(" ", order));
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: stdlib. `PriorityQueue<TElement, TPriority>` stores each element with a separate priority and dequeues the lowest priority first.
- `TryDequeue` returns `false` when the queue is empty.
- A reversed `Comparer` makes a max-heap.
