```python run id=one-call-for-an-order
import json


def cents(n):
    return f"{n // 100}.{n % 100:02d}"


# Fine-grained domain objects: small objects with small methods.
class Address:
    def __init__(self, street, city):
        self._street, self._city = street, city

    def street(self):
        return self._street

    def city(self):
        return self._city

    def move_to(self, street, city):
        self._street, self._city = street, city


class Customer:
    def __init__(self, name, address):
        self._name, self._address = name, address

    def name(self):
        return self._name

    def address(self):
        return self._address


class Line:
    def __init__(self, item, qty, price):
        self.item, self.qty, self.price = item, qty, price

    def amount(self):
        return self.qty * self.price


class Order:
    def __init__(self, number, customer, lines):
        self.number, self.customer, self.lines = number, customer, lines

    def total(self):
        return sum(line.amount() for line in self.lines)


# Remote facade: coarse-grained methods over the fine-grained objects, with no domain logic.
class OrderFacade:
    def __init__(self, orders):
        self._orders = orders

    def get_order(self, number):
        order = self._orders[number]
        address = order.customer.address()
        return {
            "number": order.number,
            "customer": order.customer.name(),
            "ship_to": f"{address.street()}, {address.city()}",
            "lines": [{"item": l.item, "qty": l.qty, "price": l.price} for l in order.lines],
            "total": order.total(),
        }

    def change_address(self, number, street, city):
        self._orders[number].customer.address().move_to(street, city)


# The network: each call is counted, and only data crosses it, as JSON text.
class Network:
    def __init__(self, server):
        self.server, self.calls = server, 0

    def call(self, method, *args):
        self.calls += 1
        request = json.dumps([method, args])
        name, params = json.loads(request)
        reply = json.dumps(getattr(self.server, name)(*params))
        return json.loads(reply)


ann = Customer("Ann", Address("1 Elm Street", "Leeds"))
orders = {7: Order(7, ann, [Line("lamp", 2, 2500), Line("desk", 1, 7500), Line("pen", 4, 500)])}
net = Network(OrderFacade(orders))

# The client: one remote call gets all the data that the screen needs.
view = net.call("get_order", 7)
print(f"order {view['number']} for {view['customer']}: {len(view['lines'])} lines, total {cents(view['total'])}")
for line in view["lines"]:
    print(f"  {line['item']} x{line['qty']} at {cents(line['price'])}")
print("ship to:", view["ship_to"])

net.call("change_address", 7, "2 Oak Road", "York")
view["ship_to"] = "not sent back"  # the client has a copy; this changes nothing on the server
print("ship to:", net.call("get_order", 7)["ship_to"])
print("remote calls:", net.calls)
```

```output
order 7 for Ann: 3 lines, total 145.00
  lamp x2 at 25.00
  desk x1 at 75.00
  pen x4 at 5.00
ship to: 1 Elm Street, Leeds
ship to: 2 Oak Road, York
remote calls: 3
```

- Verdict: idiom. `OrderFacade` has two coarse-grained methods and no domain logic. `get_order` reads the fine-grained objects and returns one dictionary with all the data that the screen needs. `change_address` passes one call to the address object.
- `Network` stands for the remote connection. It counts the calls, and it changes each request and each reply into JSON text and back, so only data crosses it. The client gets a copy, which is why the change to `view` has no effect on the server.
