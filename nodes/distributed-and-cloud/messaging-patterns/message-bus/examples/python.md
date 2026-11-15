```python run id=add-a-program
# The canonical message of the bus: a dict with a type and the shared fields.
subscribers = {}


def subscribe(msg_type, handler):
    subscribers.setdefault(msg_type, []).append(handler)


def publish(msg):
    print(f"bus: {msg['type']} order={msg['order']} amount_cents={msg['amount_cents']}")
    for handler in subscribers.get(msg["type"], []):
        handler(msg)


# Each adapter translates between the canonical message and the format of one program.
def shop_adapter(line):  # the shop writes "order 42 total 19.99"
    print("shop sends:", line)
    _, order, _, total = line.split()
    units, cents = total.split(".")
    publish({"type": "OrderPlaced", "order": int(order), "amount_cents": int(units) * 100 + int(cents)})


def billing_adapter(msg):
    print(f"billing: charge {msg['amount_cents']} cents for order {msg['order']}")


def warehouse_adapter(msg):  # the warehouse reads "SHIP|42"
    print(f"warehouse: SHIP|{msg['order']}")


orders_seen = 0


def analytics_adapter(msg):
    global orders_seen
    orders_seen += 1
    print(f"analytics: order {msg['order']} counted, total orders {orders_seen}")


subscribe("OrderPlaced", billing_adapter)
subscribe("OrderPlaced", warehouse_adapter)
shop_adapter("order 42 total 19.99")

print("analytics joins the bus")
subscribe("OrderPlaced", analytics_adapter)
shop_adapter("order 43 total 5.00")
```

```output
shop sends: order 42 total 19.99
bus: OrderPlaced order=42 amount_cents=1999
billing: charge 1999 cents for order 42
warehouse: SHIP|42
analytics joins the bus
shop sends: order 43 total 5.00
bus: OrderPlaced order=43 amount_cents=500
billing: charge 500 cents for order 43
warehouse: SHIP|43
analytics: order 43 counted, total orders 1
```

- Verdict: idiom. Python has no message bus. A dict from a message type to a list of handlers is the bus, and a dict with the shared fields is the common message.
- Each adapter knows two formats: the format of its program and the common format. No adapter knows another program.
- The shop reads the amount as text and makes cents from the two parts, so no binary floating point number is on the bus.
