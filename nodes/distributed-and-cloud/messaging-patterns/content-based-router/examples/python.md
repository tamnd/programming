```python run id=route-orders
import json

# The router reads one input channel and writes to the channel of the system for each item type.
channels = {"inventory.widgets": [], "inventory.gadgets": [], "invalid": []}
routes = {"widget": "inventory.widgets", "gadget": "inventory.gadgets"}


def route(raw):
    order = json.loads(raw)
    channel = routes.get(order.get("type"), "invalid")  # an unknown type is not lost
    channels[channel].append(order["id"])


incoming = [
    {"id": "o1", "type": "widget", "qty": 3},
    {"id": "o2", "type": "gadget", "qty": 1},
    {"id": "o3", "type": "gizmo", "qty": 2},
    {"id": "o4", "type": "widget", "qty": 1},
]
for order in incoming:
    route(json.dumps(order).encode())

for name, ids in channels.items():
    print(f"{name}: {' '.join(ids)}")
```

```output
inventory.widgets: o1 o4
inventory.gadgets: o2
invalid: o3
```

- Verdict: idiom. A dict from the item type to the name of a channel is the rule table, and the second argument of `routes.get` is the default rule.
- `order.get` gives `None` for a message with no type field, so such a message also goes to the invalid channel and does not raise an error.
- With a broker, the router is a consumer of the input queue and a producer for the output queues.
