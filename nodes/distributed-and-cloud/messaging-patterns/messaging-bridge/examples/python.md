```python run id=bridge-queues
from collections import deque

LIMIT = 64 * 1024                                   # the largest message the destination takes
ROUTES = {"orders": "sb-orders", "invoices": "sb-invoices"}  # source channel -> destination channel

source = deque([
    {"id": "m1", "channel": "orders", "label": "new-order", "size": 120},
    {"id": "m2", "channel": "invoices", "label": "invoice", "size": 300},
    {"id": "m3", "channel": "orders", "label": "new-order", "size": 90},
    {"id": "m4", "channel": "orders", "label": "bulk-order", "size": 70000},
    {"id": "m5", "channel": "invoices", "label": "invoice", "size": 210},
])
dest = {name: [] for name in ROUTES.values()}
errors, forwarded = [], set()
crashed = False

while source:
    msg = source[0]  # peek: the message stays on the source until the ack
    if msg["size"] > LIMIT:
        print("%s too large for the destination (%d > %d bytes): to the bridge error queue"
              % (msg["id"], msg["size"], LIMIT))
        errors.append(msg["id"])
    elif msg["id"] in forwarded:
        print("%s again: already forwarded, ack only" % msg["id"])
    else:
        target = ROUTES[msg["channel"]]
        # the payload is not changed; only the header name changes (label -> subject)
        dest[target].append({"id": msg["id"], "subject": msg["label"], "size": msg["size"]})
        forwarded.add(msg["id"])
        print("%s %s -> %s, %d bytes, subject %s" % (msg["id"], msg["channel"], target, msg["size"], msg["label"]))
        if msg["id"] == "m3" and not crashed:
            crashed = True
            print("the bridge stops before the ack of m3; the source delivers m3 again")
            continue
    source.popleft()  # the ack

print(" | ".join("%s: %s" % (k, " ".join(m["id"] for m in v)) for k, v in dest.items())
      + " | bridge errors: " + " ".join(errors))
```

```output
m1 orders -> sb-orders, 120 bytes, subject new-order
m2 invoices -> sb-invoices, 300 bytes, subject invoice
m3 orders -> sb-orders, 90 bytes, subject new-order
the bridge stops before the ack of m3; the source delivers m3 again
m3 again: already forwarded, ack only
m4 too large for the destination (70000 > 65536 bytes): to the bridge error queue
m5 invoices -> sb-invoices, 210 bytes, subject invoice
sb-orders: m1 m3 | sb-invoices: m2 m5 | bridge errors: m4
```

- Verdict: idiom. A `deque` is the source queue, and `source[0]` reads the head without removing it, so a stop before the ack leaves the message there.
- A dict maps each source channel to its destination channel, and the bridge renames `label` to `subject` but keeps the body.
- The set `forwarded` is the de-dup store; a real bridge keeps it in durable storage and removes old ids after a time.
