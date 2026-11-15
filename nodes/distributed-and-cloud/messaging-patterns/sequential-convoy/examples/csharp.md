```csharp run id=order-sessions
using System;
using System.Collections.Generic;
using System.Linq;

const int MaxDeliveries = 3;
var arrivals = new (string Session, string Name, string Arg)[] {
    ("A", "create", ""), ("B", "create", ""), ("C", "create", ""), ("A", "add", "10"),
    ("B", "add", "x"), ("A", "add", "5"), ("C", "add", "3"), ("B", "add", "7") };
var order = new List<string>(); // the sessions in the order they first appear
var sessions = new Dictionary<string, Queue<Op>>();
foreach (var a in arrivals)
{
    if (!sessions.ContainsKey(a.Session))
    {
        order.Add(a.Session);
        sessions[a.Session] = new Queue<Op>();
    }
    sessions[a.Session].Enqueue(new Op(a.Name, a.Arg));
}

string[] workers = { "w1", "w2" };
var holding = new Dictionary<string, string?> { ["w1"] = null, ["w2"] = null };
var locked = new HashSet<string>();
var deliveries = new Dictionary<string, int>();
var totals = new Dictionary<string, int>();
var dead = new List<string>();
int outOfOrder = 0;

for (int rnd = 1; sessions.Values.Any(q => q.Count > 0); rnd++)
{
    foreach (var w in workers)
    {
        if (holding[w] is null)
        {
            var free = order.FirstOrDefault(s => sessions[s].Count > 0 && !locked.Contains(s));
            if (free is null)
            {
                continue;
            }
            locked.Add(free);
            holding[w] = free;
            Console.WriteLine($"{rnd} {w} locks {free}");
        }
        var sid = holding[w]!;
        var op = sessions[sid].Peek();
        var label = op.Arg == "" ? $"{sid} {op.Name}" : $"{sid} {op.Name} {op.Arg}";
        bool ok = true;
        if (op.Name == "create")
        {
            totals[sid] = 0;
        }
        else if (!totals.ContainsKey(sid))
        {
            outOfOrder++;
        }
        else if (int.TryParse(op.Arg, out var n))
        {
            totals[sid] += n;
        }
        else
        {
            ok = false;
        }
        if (ok)
        {
            Console.WriteLine($"{rnd} {w} {label}");
            sessions[sid].Dequeue();
            deliveries.Remove(sid);
        }
        else
        {
            deliveries[sid] = deliveries.GetValueOrDefault(sid) + 1;
            if (deliveries[sid] < MaxDeliveries)
            {
                Console.WriteLine($"{rnd} {w} {label} failed, delivery {deliveries[sid]}");
            }
            else
            {
                Console.WriteLine($"{rnd} {w} {label} failed, delivery {deliveries[sid]}: to the dead letter queue");
                dead.Add(label);
                sessions[sid].Dequeue();
                deliveries.Remove(sid);
            }
        }
        if (sessions[sid].Count == 0)
        {
            locked.Remove(sid);
            holding[w] = null;
        }
    }
}

var parts = order.Select(s => $"{s} {totals[s]}");
Console.WriteLine($"totals: {string.Join(", ", parts)}; dead letter: {string.Join(", ", dead)}; out of order: {outOfOrder}");

record Op(string Name, string Arg);
```

```output
1 w1 locks A
1 w1 A create
1 w2 locks B
1 w2 B create
2 w1 A add 10
2 w2 B add x failed, delivery 1
3 w1 A add 5
3 w2 B add x failed, delivery 2
4 w1 locks C
4 w1 C create
4 w2 B add x failed, delivery 3: to the dead letter queue
5 w1 C add 3
5 w2 B add 7
totals: A 15, B 7, C 3; dead letter: B add x; out of order: 0
```

- Verdict: idiom. A `Dictionary` of `Queue<Op>` holds the sessions, and a `List` keeps the order of first arrival, because the order of a `Dictionary` is not guaranteed.
- `FirstOrDefault` finds the first session that has messages and no lock.
- `int.TryParse` returns false for `x`, so the failure needs no exception; the message stays at the head of its session until the third delivery.
