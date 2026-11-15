```python run id=one-service-two-clients
class DomainError(Exception):
    pass


def cents(n):
    return f"{n // 100}.{n % 100:02d}"


# Domain layer: the objects hold the business rules.
class Order:
    def __init__(self, number, customer):
        self.number = number
        self.customer = customer
        self.lines = []

    def add(self, item, qty, price_cents):
        if qty <= 0:
            raise DomainError(f"bad quantity {qty}")
        self.lines.append((item, qty, price_cents))

    def total(self):
        subtotal = sum(qty * price for _, qty, price in self.lines)
        if self.customer["tier"] == "gold" and subtotal >= 10000:
            return subtotal - subtotal // 10
        return subtotal


# Data source and other resources, which the service layer coordinates.
class Store:
    def __init__(self):
        self.customers = {1: {"name": "Ann", "email": "ann@example.com", "tier": "gold"},
                          2: {"name": "Bob", "email": "bob@example.com", "tier": "regular"}}
        self.prices = {"lamp": 2500, "desk": 7500, "pen": 500}
        self.orders = {}
        self._saved = None

    def begin(self):
        self._saved = dict(self.orders)  # a copy of the map of orders

    def commit(self):
        self._saved = None

    def rollback(self):
        self.orders = self._saved


class Mailer:
    def __init__(self):
        self.sent = []

    def send(self, to, text):
        self.sent.append(f"{to}: {text}")


# Service layer: the operations of the application, with the transaction and the
# coordination of the responses in one place.
class OrderService:
    def __init__(self, store, mailer):
        self.store = store
        self.mailer = mailer

    def place_order(self, customer_id, items):
        self.store.begin()
        try:
            customer = self.store.customers.get(customer_id)
            if customer is None:
                raise DomainError(f"no customer {customer_id}")
            order = Order(len(self.store.orders) + 1, customer)
            self.store.orders[order.number] = order
            for item, qty in items:
                if item not in self.store.prices:
                    raise DomainError(f"no product {item}")
                order.add(item, qty, self.store.prices[item])
            self.store.commit()
        except DomainError:
            self.store.rollback()
            raise
        # Only a committed order gets an email.
        self.mailer.send(customer["email"], f"order {order.number}, total {cents(order.total())}")
        return order.number, customer["name"], order.total()


store, mailer = Store(), Mailer()
service = OrderService(store, mailer)

# Client 1: a web handler.
number, name, total = service.place_order(1, [("lamp", 2), ("desk", 1)])
print(f"web: placed order {number} for {name}, total {cents(total)}")

# Client 2: a loader that imports orders from a file.
for line_no, (customer_id, items) in enumerate([(2, [("lamp", 1), ("pen", 3)]), (2, [("pen", 1), ("chair", 1)])], 1):
    try:
        number, name, total = service.place_order(customer_id, items)
        print(f"import line {line_no}: placed order {number} for {name}, total {cents(total)}")
    except DomainError as e:
        print(f"import line {line_no}: refused: {e}")

print("orders stored:", len(store.orders))
for message in mailer.sent:
    print("email", message)
```

```output
web: placed order 1 for Ann, total 112.50
import line 1: placed order 2 for Bob, total 40.00
import line 2: refused: no product chair
orders stored: 2
email ann@example.com: order 1, total 112.50
email bob@example.com: order 2, total 40.00
```

- Verdict: idiom. `OrderService` is a plain class, and `place_order` is one operation of the application. It opens the transaction, asks the domain objects to do the work, commits, and then sends the email. The two clients only call the service and report the result.
- The store keeps a copy of its map of orders at `begin` and puts it back at `rollback`. A real service would use a database transaction instead, for example `with connection:` in `sqlite3`.
