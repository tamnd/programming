```python run id=order-events
import json

# Each event message has the CloudEvents attributes: specversion, id, source and type.
def event(eid, etype, **data):
    return json.dumps({"specversion": "1.0", "id": eid, "source": "/shop",
                       "type": "com.example.order." + etype, "data": data}).encode()


def subscriber(name, handlers):
    seen = set()

    def deliver(raw):
        e = json.loads(raw)
        key = (e["source"], e["id"])
        if key in seen:
            print(f"{name}: skip duplicate {e['id']}")
            return
        seen.add(key)
        handler = handlers.get(e["type"])
        if handler is None:
            print(f"{name}: ignore {e['type']}")
            return
        print(f"{name}: {handler(e['data'])}")
    return deliver


topic = [  # a publish-subscribe channel: each subscriber gets a copy of each event
    subscriber("billing", {
        "com.example.order.placed": lambda d: f"charge order {d['order']}, {d['cents']} cents",
        "com.example.order.cancelled": lambda d: f"refund order {d['order']}"}),
    subscriber("email", {
        "com.example.order.placed": lambda d: f"mail the confirmation of order {d['order']}"}),
]

events = [
    event("e1", "placed", order=41, cents=1999),
    event("e2", "placed", order=42, cents=500),
    event("e2", "placed", order=42, cents=500),  # sent again after a network error
    event("e3", "cancelled", order=41),
]
for raw in events:
    for deliver in topic:
        deliver(raw)
```

```output
billing: charge order 41, 1999 cents
email: mail the confirmation of order 41
billing: charge order 42, 500 cents
email: mail the confirmation of order 42
billing: skip duplicate e2
email: skip duplicate e2
billing: refund order 41
email: ignore com.example.order.cancelled
```

- Verdict: idiom. Python has no publish-subscribe channel in the standard library. A list of subscriber functions is the topic, and `json` turns each event into bytes and back.
- Each subscriber keeps a set of `(source, id)` pairs, so it handles a copy of an event only once.
- A dict from the `type` of an event to a function is the set of events that a subscriber cares about. Other types are ignored, not errors.
