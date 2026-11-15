```python run id=logical-and-physical-layers
import sqlite3


class AccessError(Exception):
    # The error that the application kernel sees, whatever the database says.
    pass


# ---- Physical access layer: knows the tables and the SQL. ----

class Database:
    # Wraps the connection and runs the statements.
    def __init__(self, script):
        self.conn = sqlite3.connect(":memory:")
        self.conn.executescript(script)

    def query(self, sql, args):
        return self.conn.execute(sql, args).fetchall()

    def execute(self, sql, args):
        self.conn.execute(sql, args)

    def commit(self):
        self.conn.commit()


NORMALIZED = """
create table customer (id integer primary key, name text);
create table orders (id integer primary key, customer_id integer);
create table order_line (order_id integer, item text, qty integer);
insert into customer values (1, 'Acme Ltd');
insert into orders values (10, 1);
insert into order_line values (10, 'bolt', 2), (10, 'nut', 5);
"""

# After tuning: the customer name is copied into the orders table.
DENORMALIZED = """
create table orders (id integer primary key, customer_name text);
create table order_line (order_id integer, item text, qty integer);
insert into orders values (10, 'Acme Ltd');
insert into order_line values (10, 'bolt', 2), (10, 'nut', 5);
"""


class OrderView:
    # The protocol of a physical view of orders: read an order, write a line.
    def __init__(self, db):
        self.db = db

    def write_line(self, order_id, item, qty):
        self.db.execute("update order_line set qty = ? where order_id = ? and item = ?", (qty, order_id, item))


class NormalizedOrderView(OrderView):
    # Reads an order from the normalized tables.
    tables = "customer, orders, order_line"

    def read(self, order_id):
        head = self.db.query("select c.name from orders o join customer c on c.id = o.customer_id where o.id = ?", (order_id,))
        lines = self.db.query("select item, qty from order_line where order_id = ? order by item", (order_id,))
        return head, lines


class DenormalizedOrderView(OrderView):
    # Reads an order from the tuned tables: one table less to read.
    tables = "orders, order_line"

    def read(self, order_id):
        head = self.db.query("select customer_name from orders where id = ?", (order_id,))
        lines = self.db.query("select item, qty from order_line where order_id = ? order by item", (order_id,))
        return head, lines


# ---- Logical access layer: the interface of the application kernel. ----

class Order:
    # The logical view of an order. It has no SQL in it.
    def __init__(self, order_id, customer, lines):
        self.id, self.customer, self.lines = order_id, customer, dict(lines)
        self.changed = set()

    def set_qty(self, item, qty):
        self.lines[item] = qty
        self.changed.add(item)


class AccessLayer:
    def __init__(self, physical):
        self.physical, self.cache, self.reads = physical, {}, 0

    def start(self):
        self.cache, self.reads = {}, 0

    def get_order(self, order_id):
        if order_id not in self.cache:
            head, lines = self.physical.read(order_id)
            self.reads += 1
            if not head:
                raise AccessError(f"no order {order_id}")
            self.cache[order_id] = Order(order_id, head[0][0], lines)
        return self.cache[order_id]

    def commit(self):
        written = []
        for order in self.cache.values():
            for item in sorted(order.changed):
                self.physical.write_line(order.id, item, order.lines[item])
                written.append(f"order {order.id} {item} x {order.lines[item]}")
            order.changed.clear()
        self.physical.db.commit()
        return written


# ---- Application kernel: uses only the logical access layer. ----

def invoice(layer, order_id):
    o = layer.get_order(order_id)
    return " | ".join([o.customer] + [f"{item} x {qty}" for item, qty in sorted(o.lines.items())])


def run(name, script, view_class):
    layer = AccessLayer(view_class(Database(script)))
    print("physical layout:", name)
    layer.start()
    print("invoice 10:", invoice(layer, 10))
    print("invoice 10:", invoice(layer, 10))
    print(f"physical reads: {layer.reads}, tables {view_class.tables}")
    layer.get_order(10).set_qty("bolt", 3)
    print("commit:", ", ".join(layer.commit()))
    layer.start()
    print("invoice 10 in a new transaction:", invoice(layer, 10))
    try:
        invoice(layer, 99)
    except AccessError as e:
        print("error:", e)


run("normalized", NORMALIZED, NormalizedOrderView)
run("denormalized", DENORMALIZED, DenormalizedOrderView)
```

```output
physical layout: normalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables customer, orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
physical layout: denormalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
```

- Verdict: idiom. The physical views hold all the SQL. `AccessLayer` is the logical layer, with the start and commit of a transaction and a cache of the views read in it. `invoice` is the application kernel and has no SQL.
- `AccessError` replaces the empty result of the database, so the kernel does not depend on how SQLite reports a missing row.
- The two physical views read the same logical order from two table layouts. Only the class given to `AccessLayer` changes.
