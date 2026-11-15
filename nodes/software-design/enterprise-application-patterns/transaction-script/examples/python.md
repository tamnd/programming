```python run id=one-script-per-request
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text, tier text);
create table product (id integer primary key, name text, price_cents integer);
create table orders (id integer primary key, customer_id integer);
create table order_line (order_id integer, product_id integer, qty integer);
insert into customer values (1, 'Ann', 'gold'), (2, 'Bob', 'regular');
insert into product values (1, 'lamp', 2500), (2, 'desk', 7500), (3, 'pen', 500);
""")


class ScriptError(Exception):
    pass


def cents(n):
    return f"{n // 100}.{n % 100:02d}"


# Transaction script 1: place an order.
def place_order(customer_id, items):
    with db:  # one database transaction for the whole script
        row = db.execute("select name from customer where id = ?", (customer_id,)).fetchone()
        if row is None:
            raise ScriptError(f"no customer {customer_id}")
        for product_id, qty in items:
            if qty <= 0:
                raise ScriptError(f"bad quantity {qty}")
            if db.execute("select 1 from product where id = ?", (product_id,)).fetchone() is None:
                raise ScriptError(f"no product {product_id}")
        order_id = db.execute("insert into orders (customer_id) values (?)", (customer_id,)).lastrowid
        db.executemany(
            "insert into order_line values (?, ?, ?)",
            [(order_id, p, q) for p, q in items],
        )
    return order_id, row[0]


# Transaction script 2: show the total of an order.
def order_total(order_id):
    row = db.execute(
        """select c.tier, sum(l.qty * p.price_cents)
           from orders o join customer c on c.id = o.customer_id
           join order_line l on l.order_id = o.id join product p on p.id = l.product_id
           where o.id = ?""",
        (order_id,),
    ).fetchone()
    if row[1] is None:
        raise ScriptError(f"no order {order_id}")
    tier, subtotal = row
    total = subtotal
    if tier == "gold" and subtotal >= 10000:
        total = subtotal - subtotal // 10
    return subtotal, total


for customer_id, items in [(1, [(1, 2), (2, 1)]), (2, [(1, 1), (3, 3)])]:
    order_id, name = place_order(customer_id, items)
    print(f"placed order {order_id} for {name}")
for order_id in (1, 2):
    subtotal, total = order_total(order_id)
    print(f"order {order_id}: subtotal {cents(subtotal)}, total {cents(total)}")
for customer_id, items in [(1, [(9, 1)]), (2, [(1, 0)])]:
    try:
        place_order(customer_id, items)
    except ScriptError as e:
        print("error:", e)
print("orders in the database:", db.execute("select count(*) from orders").fetchone()[0])
```

```output
placed order 1 for Ann
placed order 2 for Bob
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
error: no product 9
error: bad quantity 0
orders in the database: 2
```

- Verdict: idiom. Each script is a plain function. `place_order` runs in one transaction with `with db:`, so an error in the middle writes nothing. `order_total` does the calculation in one SQL query with a join and a sum, and then applies the discount rule in Python.
- The scripts share the helper `cents` and the exception `ScriptError`. Shared subtasks of this kind are normal in a transaction script design.
