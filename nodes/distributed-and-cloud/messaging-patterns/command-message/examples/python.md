```python run id=dispatch-commands
import json
from collections import deque

channel = deque()  # the channel carries bytes; it does not know what is in them


def send(command, **args):
    channel.append(json.dumps({"command": command, **args}).encode())


stock = {"A1": 5}


def reserve_stock(sku, qty):
    if stock[sku] < qty:
        return f"reserve {qty} of {sku}: not enough stock, {stock[sku]} left"
    stock[sku] -= qty
    return f"reserve {qty} of {sku}: {stock[sku]} left"


def charge_card(order, cents):
    return f"charge {cents} cents for order {order}: done"


HANDLERS = {"ReserveStock": reserve_stock, "ChargeCard": charge_card}

send("ReserveStock", sku="A1", qty=2)
send("ChargeCard", order=42, cents=1999)
send("ReserveStock", sku="A1", qty=9)

while channel:
    args = json.loads(channel.popleft())
    command = args.pop("command")
    print(HANDLERS[command](**args))
```

```output
reserve 2 of A1: 3 left
charge 1999 cents for order 42: done
reserve 9 of A1: not enough stock, 3 left
```

- Verdict: stdlib. `json` turns each command into bytes and back, and a dict from the name of a command to a function is the dispatcher.
- The channel holds bytes, as a broker does. It cannot see the command, so any program that can read JSON can send to the receiver or read from the channel.
- `HANDLERS[command]` raises `KeyError` for an unknown command. A receiver on a broker would catch it and move the message to an invalid message channel.
