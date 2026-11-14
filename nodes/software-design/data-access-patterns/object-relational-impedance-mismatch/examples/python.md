```python run id=map-a-customer
import sqlite3
from dataclasses import dataclass, field

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text, street text, city text);
create table orders (
  id integer primary key, customer_id integer references customer (id),
  total integer, kind text, card text);
insert into customer values (1, 'Ann', 'Main St 1', 'Oslo'), (2, 'Bo', 'High St 9', 'Rome');
insert into orders values (10, 1, 30, 'card', '4242'), (11, 1, 20, 'cash', null),
  (12, 2, 45, 'card', '1881');
""")
statements = []
db.set_trace_callback(statements.append)


@dataclass
class Address:  # A value object: it has no table and no key.
    street: str
    city: str


@dataclass(eq=False)
class Customer:
    id: int
    name: str
    address: Address
    orders: list = field(default_factory=list)


@dataclass(eq=False)
class Order:
    id: int
    total: int
    customer: Customer


@dataclass(eq=False)
class CardOrder(Order):
    card: str


class CashOrder(Order):
    pass


def yn(flag):
    return "yes" if flag else "no"


def load(id):
    sql = "select name, street, city from customer where id = ?"
    name, street, city = db.execute(sql, (id,)).fetchone()
    # Granularity: one row becomes two objects.
    c = Customer(id, name, Address(street, city))
    sql = "select id, total, kind, card from orders where customer_id = ? order by id"
    for oid, total, kind, card in db.execute(sql, (id,)):
        # Subtypes: a column chooses the class.
        o = CardOrder(oid, total, c, card) if kind == "card" else CashOrder(oid, total, c)
        # Associations: the foreign key becomes two references.
        c.orders.append(o)
    return c


ann, bo = load(1), load(2)
a = ann.address
print(f"granularity: customer row 1 -> Customer {ann.name} + Address {a.street}, {a.city}")
kinds = [f"order {o.id} {type(o).__name__}" for o in ann.orders]
print("subtypes:", " | ".join(kinds), "| card", ann.orders[0].card)
ids = ", ".join(str(o.id) for o in ann.orders)
print(f"associations: order 10 -> {ann.orders[0].customer.name} | Ann -> orders {ids}")

# In the table, one update moves the order. In memory, both sides must change.
moved = ann.orders[1]
moved.customer = bo
print("moved order 11 on one side: Ann lists 11:", yn(moved in ann.orders),
      "| Bo lists 11:", yn(moved in bo.orders))

again = load(1)
print("identity: same object:", yn(again is ann), "| same key:", yn(again.id == ann.id))

statements.clear()
keys = [id for (id,) in db.execute("select id from customer order by id")]
walk = ", ".join(f"{c.name} {sum(o.total for o in c.orders)}" for c in map(load, keys))
print(f"navigation: walk {walk} in {len(statements)} statements")

statements.clear()
rows = db.execute(
    "select name, sum(total) from customer"
    " join orders on orders.customer_id = customer.id"
    " group by customer.id order by customer.id"
).fetchall()
join = ", ".join(f"{name} {total}" for name, total in rows)
print(f"navigation: join {join} in {len(statements)} statement")
```

```output
granularity: customer row 1 -> Customer Ann + Address Main St 1, Oslo
subtypes: order 10 CardOrder | order 11 CashOrder | card 4242
associations: order 10 -> Ann | Ann -> orders 10, 11
moved order 11 on one side: Ann lists 11: yes | Bo lists 11: no
identity: same object: no | same key: yes
navigation: walk Ann 50, Bo 45 in 5 statements
navigation: join Ann 50, Bo 45 in 1 statement
```

The program uses SQLite. One row of `customer` becomes a `Customer` and an `Address`, and the `kind` column chooses `CardOrder` or `CashOrder`. The classes use `eq=False`, so `==` and `in` compare identity, as for any class without its own `__eq__`. The trace callback counts the SQL statements: the walk sends one statement for the keys and two for each customer, and the join sends one.
