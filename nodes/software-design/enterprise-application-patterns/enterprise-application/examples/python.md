```python run id=three-layers
import copy


# Data source layer: the stored data, and a transaction that keeps all or none of a change.
class Store:
    def __init__(self):
        self.tables = {"stock": {"widget": 10}, "orders": []}

    def transaction(self, change):
        saved = copy.deepcopy(self.tables)
        try:
            change(self.tables)
            return True
        except ValueError:
            self.tables = saved  # roll back
            return False


# Domain layer: the business rules, with a special deal for one customer.
PRICE_CENTS = 1000


def price_cents(customer, quantity):
    total = PRICE_CENTS * quantity
    if customer == "acme" and quantity >= 3:  # a deal that sales made with one customer
        total = total * 90 // 100
    return total


def place_order(store, customer, item, quantity):
    def change(tables):
        tables["orders"].append((customer, item, quantity, price_cents(customer, quantity)))
        if tables["stock"][item] < quantity:
            raise ValueError("not enough stock")
        tables["stock"][item] -= quantity

    return store.transaction(change)


def money(cents):
    return f"{cents // 100}.{cents % 100:02d}"


# Presentation layer: two screens show the same data in different ways.
def order_screen(store):
    for customer, item, quantity, cents in store.tables["orders"]:
        print(f"order screen: {customer} bought {quantity} x {item} for {money(cents)}")


def stock_screen(store):
    for item, count in store.tables["stock"].items():
        print(f"stock screen: {item}: {count} in stock, orders: {len(store.tables['orders'])}")


store = Store()
print("acme orders 3:", "kept" if place_order(store, "acme", "widget", 3) else "rolled back")
print("bolt orders 9:", "kept" if place_order(store, "bolt", "widget", 9) else "rolled back")
order_screen(store)
stock_screen(store)
```

```output
acme orders 3: kept
bolt orders 9: rolled back
order screen: acme bought 3 x widget for 27.00
stock screen: widget: 7 in stock, orders: 1
```

- Verdict: library. Django, Flask and SQLAlchemy are common choices for the screens and the data. The standard library has `sqlite3`, and its connection commits or rolls back a transaction as a context manager. The example keeps the three layers in one file and copies the tables for a rollback.
