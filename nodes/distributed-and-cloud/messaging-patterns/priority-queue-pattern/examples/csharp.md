```csharp run id=prio-dispatch
var level = new Dictionary<string, int> { ["high"] = 0, ["normal"] = 1, ["low"] = 2 };
(int Tick, string Id, string Prio)[] arrivals =
[
    (0, "l1", "low"), (0, "l2", "low"), (0, "h1", "high"), (1, "n1", "normal"),
    (1, "h2", "high"), (2, "l3", "low"), (3, "h3", "high"), (4, "n2", "normal"),
];

// PriorityQueue does not keep FIFO order for equal priorities, so the arrival number breaks ties.
var queue = new PriorityQueue<(int Arrived, string Id, string Prio), (int Level, long Seq)>();
var order = new List<string>();
long seq = 0;
int i = 0;
for (int t = 0; i < arrivals.Length || queue.Count > 0; t++)
{
    while (i < arrivals.Length && arrivals[i].Tick == t)
    {
        var a = arrivals[i++];
        queue.Enqueue((t, a.Id, a.Prio), (level[a.Prio], seq++));
    }
    if (queue.TryDequeue(out var m, out _))
    {
        Console.WriteLine($"tick {t}: {m.Id} ({m.Prio}, waited {t - m.Arrived})");
        order.Add(m.Id);
    }
}
Console.WriteLine("order: " + string.Join(" ", order));
Console.WriteLine($"l1 before l2: {(order.IndexOf("l1") < order.IndexOf("l2")).ToString().ToLowerInvariant()}");
```

```output
tick 0: h1 (high, waited 0)
tick 1: h2 (high, waited 0)
tick 2: n1 (normal, waited 1)
tick 3: h3 (high, waited 0)
tick 4: n2 (normal, waited 0)
tick 5: l1 (low, waited 5)
tick 6: l2 (low, waited 6)
tick 7: l3 (low, waited 5)
order: h1 h2 n1 h3 n2 l1 l2 l3
l1 before l2: true
```

- Verdict: idiom. `PriorityQueue<TElement, TPriority>` in .NET keeps the priority apart from the element, and a value tuple `(Level, Seq)` is a priority that compares field by field.
- The .NET documentation says that the queue does not guarantee first-in, first-out order for equal priorities, so the arrival number is part of the priority.
- `TryDequeue` returns the element and its priority, and it returns false when the queue is empty.
