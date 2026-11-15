```python run id=split-order
from dataclasses import dataclass


@dataclass
class Line:
    kind: str
    qty: int


orders = {"A7": [Line("widget", 2), Line("gadget", 1), Line("widget", 5)], "B9": []}
channel = {"widget": "inventory.widgets", "gadget": "inventory.gadgets"}


def split(order_id, lines):
    """One message for each line, with the headers that an aggregator needs."""
    if not lines:
        # One message with size 0, so that a later aggregator is not left waiting.
        return [{"correlation_id": order_id, "seq": 0, "size": 0, "line": None}]
    return [{"correlation_id": order_id, "seq": n, "size": len(lines), "line": line}
            for n, line in enumerate(lines, 1)]


for order_id, lines in orders.items():
    print(f"order {order_id} has {len(lines)} lines")
    for m in split(order_id, lines):
        if m["line"] is None:
            print(f"  {order_id} 0/0 empty order -> orders.empty")
            continue
        line = m["line"]
        print(f"  {order_id} {m['seq']}/{m['size']} {line.kind} x{line.qty} -> {channel[line.kind]}")
```

```output
order A7 has 3 lines
  A7 1/3 widget x2 -> inventory.widgets
  A7 2/3 gadget x1 -> inventory.gadgets
  A7 3/3 widget x5 -> inventory.widgets
order B9 has 0 lines
  B9 0/0 empty order -> orders.empty
```

- Verdict: idiom. A list comprehension with `enumerate(lines, 1)` makes one message for each line, with the sequence number from 1 and the size of the list.
- The order id is the correlation id of each part, so that an aggregator can find all the parts of one order.
- For a large order, a generator with `yield` sends the parts one at a time, but then the size is not known until the end.
