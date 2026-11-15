```python run id=one-object-for-all-orders
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text, tier text);
create table orders (id integer primary key, customer_id integer);
create table order_line (order_id integer, item text, qty integer, price_cents integer);
insert into customer values (1, 'Ann', 'gold'), (2, 'Bob', 'regular'), (3, 'Cy', 'staff');
insert into orders values (1, 1), (2, 2), (3, 3);
insert into order_line values (1, 'lamp', 2, 2500), (1, 'desk', 1, 7500),
  (2, 'lamp', 1, 2500), (2, 'pen', 3, 500), (3, 'lamp', 1, 2500), (3, 'pen', 3, 500);
""")


class ModuleError(Exception):
    pass


def cents(n):
    return f"{n // 100}.{n % 100:02d}"


# The record set: an in-memory copy of the rows of several tables.
def load_record_set():
    record_set = {}
    for table in ("customer", "orders", "order_line"):
        cursor = db.execute(f"select * from {table}")
        names = [d[0] for d in cursor.description]
        record_set[table] = [dict(zip(names, row)) for row in cursor]
    return record_set


# One table module for each table. One instance handles all the rows of its table.
class CustomerModule:
    def __init__(self, record_set):
        self.rows = record_set["customer"]

    def row(self, customer_id):
        for row in self.rows:
            if row["id"] == customer_id:
                return row
        raise ModuleError(f"no customer {customer_id}")

    def names_with_tier(self, tier):
        return [row["name"] for row in self.rows if row["tier"] == tier]


class OrderModule:
    def __init__(self, record_set, customers):
        self.rows = record_set["orders"]
        self.lines = record_set["order_line"]
        self.customers = customers

    def row(self, order_id):
        for row in self.rows:
            if row["id"] == order_id:
                return row
        raise ModuleError(f"no order {order_id}")

    def subtotal(self, order_id):
        self.row(order_id)
        return sum(l["qty"] * l["price_cents"] for l in self.lines if l["order_id"] == order_id)

    def total(self, order_id):
        subtotal = self.subtotal(order_id)
        tier = self.customers.row(self.row(order_id)["customer_id"])["tier"]
        if tier == "gold" and subtotal >= 10000:
            return subtotal - subtotal // 10
        if tier == "staff":
            return subtotal - subtotal // 5
        return subtotal

    def count_over(self, limit):
        return sum(1 for row in self.rows if self.total(row["id"]) > limit)


record_set = load_record_set()
customers = CustomerModule(record_set)
orders = OrderModule(record_set, customers)
print(f"record set: {len(record_set['customer'])} customers, "
      f"{len(record_set['orders'])} orders, {len(record_set['order_line'])} order lines")
for order_id in (1, 2, 3):
    print(f"order {order_id}: subtotal {cents(orders.subtotal(order_id))}, "
          f"total {cents(orders.total(order_id))}")
print("gold customers:", ", ".join(customers.names_with_tier("gold")))
print("orders over 35.00:", orders.count_over(3500))
try:
    orders.total(9)
except ModuleError as e:
    print("error:", e)
```

```output
record set: 3 customers, 3 orders, 6 order lines
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
order 3: subtotal 40.00, total 32.00
gold customers: Ann
orders over 35.00: 2
error: no order 9
```

- Verdict: idiom. `load_record_set` copies the rows of three tables into lists of dictionaries, which is the record set. `CustomerModule` and `OrderModule` each take the record set once, and their methods take the id of the row to work on.
- The discount rule is a chain of conditions on the tier in `OrderModule.total`. A table module has no object for each customer, so it cannot use polymorphism for the tiers.
