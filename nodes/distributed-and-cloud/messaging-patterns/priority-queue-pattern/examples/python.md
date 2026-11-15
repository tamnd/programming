```python run id=prio-dispatch
import heapq
from itertools import count

LEVEL = {"high": 0, "normal": 1, "low": 2}  # a smaller number goes first
# (tick, id, priority) in the order the messages arrive
arrivals = [(0, "l1", "low"), (0, "l2", "low"), (0, "h1", "high"), (1, "n1", "normal"),
            (1, "h2", "high"), (2, "l3", "low"), (3, "h3", "high"), (4, "n2", "normal")]

queue = []
seq = count()  # the arrival number breaks ties, so equal priorities stay FIFO
i, t, order = 0, 0, []
while i < len(arrivals) or queue:
    while i < len(arrivals) and arrivals[i][0] == t:
        _, mid, prio = arrivals[i]
        heapq.heappush(queue, (LEVEL[prio], next(seq), t, mid, prio))
        i += 1
    if queue:
        _, _, arrived, mid, prio = heapq.heappop(queue)
        print("tick %d: %s (%s, waited %d)" % (t, mid, prio, t - arrived))
        order.append(mid)
    t += 1
print("order: " + " ".join(order))
print("l1 before l2: %s" % str(order.index("l1") < order.index("l2")).lower())
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

- Verdict: idiom. `heapq` keeps a list as a binary heap, and a tuple key compares field by field, so `(level, seq)` gives the priority first and the arrival order second.
- `itertools.count` gives the arrival number; without it, two messages of the same priority would compare by the next fields, and the order would depend on the ids.
- A smaller number pops first in `heapq`, so high has the level 0; RabbitMQ uses the opposite rule, where a larger number is a higher priority.
