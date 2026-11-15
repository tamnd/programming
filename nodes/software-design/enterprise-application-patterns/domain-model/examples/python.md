```python run id=orders-that-know-their-rules
class DomainError(Exception):
    pass


def cents(n):
    return f"{n // 100}.{n % 100:02d}"


# Each kind of customer knows its own discount rule.
class Customer:
    kind = "regular"

    def __init__(self, name):
        self.name = name

    def discount(self, subtotal):
        return 0


class GoldCustomer(Customer):
    kind = "gold"

    def discount(self, subtotal):
        return subtotal // 10 if subtotal >= 10000 else 0


class StaffCustomer(Customer):
    kind = "staff"

    def discount(self, subtotal):
        return subtotal // 5


class Product:
    def __init__(self, name, price_cents):
        self.name = name
        self.price_cents = price_cents


class OrderLine:
    def __init__(self, product, qty):
        if qty <= 0:
            raise DomainError(f"bad quantity {qty}")
        self.product = product
        self.qty = qty

    def amount(self):
        return self.qty * self.product.price_cents


class Order:
    def __init__(self, number, customer):
        self.number = number
        self.customer = customer
        self._lines = []
        self._placed = False

    def add(self, product, qty):
        if self._placed:
            raise DomainError(f"order {self.number} is placed, so it takes no new lines")
        self._lines.append(OrderLine(product, qty))

    def place(self):
        if not self._lines:
            raise DomainError(f"order {self.number} has no lines")
        self._placed = True

    def subtotal(self):
        return sum(line.amount() for line in self._lines)

    def total(self):
        subtotal = self.subtotal()
        return subtotal - self.customer.discount(subtotal)


lamp, desk, pen = Product("lamp", 2500), Product("desk", 7500), Product("pen", 500)
orders = [
    Order(1, GoldCustomer("Ann")),
    Order(2, Customer("Bob")),
    Order(3, StaffCustomer("Cy")),
]
orders[0].add(lamp, 2)
orders[0].add(desk, 1)
for order in orders[1:]:
    order.add(lamp, 1)
    order.add(pen, 3)
for order in orders:
    order.place()
    print(f"order {order.number} for {order.customer.name} ({order.customer.kind}): "
          f"subtotal {cents(order.subtotal())}, total {cents(order.total())}")
for action in (lambda: orders[0].add(pen, 1), lambda: Order(4, Customer("Di")).place(),
               lambda: Order(5, Customer("Ed")).add(pen, 0)):
    try:
        action()
    except DomainError as e:
        print("error:", e)
```

```output
order 1 for Ann (gold): subtotal 125.00, total 112.50
order 2 for Bob (regular): subtotal 40.00, total 40.00
order 3 for Cy (staff): subtotal 40.00, total 32.00
error: order 1 is placed, so it takes no new lines
error: order 4 has no lines
error: bad quantity 0
```

- Verdict: idiom. Each kind of customer is a subclass that overrides `discount`, so `Order.total` has no condition on the kind. The order keeps its lines in `_lines`; the leading underscore tells other code not to change the list directly, but Python does not enforce it.
- The rules raise `DomainError`. The checks are in the objects, so every caller gets them.
