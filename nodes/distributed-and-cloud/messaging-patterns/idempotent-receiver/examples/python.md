```python run id=payment-receiver
from collections import OrderedDict

WINDOW = 3
deliveries = [
    ("pay-1", 17, 1250), ("pay-2", 18, 800), ("pay-1", 17, 1250),
    ("pay-3", 17, 300), ("pay-4", 19, 500), ("pay-5", 20, 100),
    ("pay-2", 18, 800),
]


class PaymentReceiver:
    def __init__(self, window):
        self.window = window
        self.seen = OrderedDict()  # message id -> None, oldest first
        self.totals = {}
        self.ever = set()  # only to explain the output; a real receiver has no such set

    def receive(self, mid, order, cents):
        if mid in self.seen:
            return f"{mid}: duplicate, ignored"
        self.totals[order] = self.totals.get(order, 0) + cents
        self.seen[mid] = None
        if len(self.seen) > self.window:
            self.seen.popitem(last=False)
        note = f" (applied twice: the id left the window of {self.window} ids)" if mid in self.ever else ""
        self.ever.add(mid)
        return f"{mid}: order {order} +{cents}, total {self.totals[order]}{note}"


r = PaymentReceiver(WINDOW)
for d in deliveries:
    print(r.receive(*d))
```

```output
pay-1: order 17 +1250, total 1250
pay-2: order 18 +800, total 800
pay-1: duplicate, ignored
pay-3: order 17 +300, total 1550
pay-4: order 19 +500, total 500
pay-5: order 20 +100, total 100
pay-2: order 18 +800, total 1600 (applied twice: the id left the window of 3 ids)
```

- Verdict: idiom. An `OrderedDict` keeps the ids in the order of arrival, so `popitem(last=False)` removes the oldest id when the window is full.
- The id is added only after the effect, and both are in memory here; with a database, the update of the total and the insert of the id belong in one transaction.
- The set `ever` exists only to explain the output; a real receiver cannot know about an id that it has forgotten.
