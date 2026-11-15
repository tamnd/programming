```python run id=order-sessions
from collections import deque

# (session id, operation, argument) in the order they arrive on the queue
arrivals = [("A", "create", ""), ("B", "create", ""), ("C", "create", ""), ("A", "add", "10"),
            ("B", "add", "x"), ("A", "add", "5"), ("C", "add", "3"), ("B", "add", "7")]
MAX_DELIVERIES = 3

sessions = {}  # a dict keeps the order in which the sessions first appear
for sid, op, arg in arrivals:
    sessions.setdefault(sid, deque()).append((op, arg))

locks = {}                          # session id -> worker that holds its lock
holding = {"w1": None, "w2": None}  # worker -> session id
deliveries = {}
totals, dead, out_of_order = {}, [], 0
rnd = 0
while any(sessions.values()):
    rnd += 1
    for w in holding:
        if holding[w] is None:
            free = next((s for s, q in sessions.items() if q and s not in locks), None)
            if free is None:
                continue
            locks[free], holding[w] = w, free
            print("%d %s locks %s" % (rnd, w, free))
        sid = holding[w]
        op, arg = sessions[sid][0]
        try:
            if op == "create":
                totals[sid] = 0
            elif sid not in totals:
                out_of_order += 1
            else:
                totals[sid] += int(arg)
            print("%d %s %s %s %s" % (rnd, w, sid, op, arg) if arg else "%d %s %s %s" % (rnd, w, sid, op))
            sessions[sid].popleft()
            deliveries.pop(sid, None)
        except ValueError:
            deliveries[sid] = deliveries.get(sid, 0) + 1
            if deliveries[sid] < MAX_DELIVERIES:
                print("%d %s %s %s %s failed, delivery %d" % (rnd, w, sid, op, arg, deliveries[sid]))
            else:
                print("%d %s %s %s %s failed, delivery %d: to the dead letter queue" % (rnd, w, sid, op, arg, deliveries[sid]))
                dead.append("%s %s %s" % (sid, op, arg))
                sessions[sid].popleft()
                deliveries.pop(sid)
        if not sessions[sid]:
            del locks[sid]
            holding[w] = None

print("totals: " + ", ".join("%s %d" % kv for kv in totals.items())
      + "; dead letter: " + ", ".join(dead) + "; out of order: %d" % out_of_order)
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

- Verdict: idiom. A dict of `deque` objects holds the sessions; a dict keeps the order in which its keys were added, so the workers lock the sessions in the order of first arrival.
- The dict `locks` maps a session to the worker that holds it, and a worker keeps its session until the session is empty.
- `int("x")` raises `ValueError`, which is the failure; the message stays at the head of its session until the third delivery.
