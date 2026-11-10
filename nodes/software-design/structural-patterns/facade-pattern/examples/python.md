```python run id=shop
# The subsystem: three classes, each with its own small job.
class Inventory:
    def __init__(self, stock):
        self._stock = dict(stock)

    def reserve(self, item):
        if self._stock.get(item, 0) == 0:
            return False
        self._stock[item] -= 1
        return True

    def release(self, item):
        self._stock[item] += 1

    def count(self, item):
        return self._stock.get(item, 0)


class Payments:
    def charge(self, card, amount):
        return card != "expired"


class Shipping:
    def __init__(self):
        self._parcels = 0

    def schedule(self, item):
        self._parcels += 1
        return self._parcels


# The facade: one function that calls the subsystem in the right order.
_inventory = Inventory({"book": 2, "lamp": 0})
_payments = Payments()
_shipping = Shipping()


def place_order(item, card, price):
    if not _inventory.reserve(item):
        return f"failed: {item} is out of stock"
    if not _payments.charge(card, price):
        _inventory.release(item)  # undo the reservation
        return f"failed: card declined, {item} back in stock"
    parcel = _shipping.schedule(item)
    return f"ok: {item}, paid {price:.2f}, parcel {parcel}"


def stock(item):
    return _inventory.count(item)


print(place_order("book", "visa", 12.50))
print(place_order("book", "expired", 12.50))
print(place_order("lamp", "visa", 30.00))
print(f"stock of book: {stock('book')}")
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: idiom. In Python, a module is the facade. A module `shop` would export `place_order` and `stock`, and keep the subsystem objects as private names with a leading underscore. An `__all__` list names the public part for `from shop import *`.
- The facade owns the order of the steps and the undo of the reservation. A client that called the three classes itself would have to repeat both.
- The subsystem classes stay public. A client that needs to read the inventory without an order can still use `Inventory`.
- `subprocess.run` is a facade in the standard library. It starts a `Popen`, waits for the process, collects its output, and kills it on a timeout.
