```python run id=four-kinds-of-lazy-load
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text, city text);
create table orders (id integer primary key, customer_id integer);
insert into customer values (1, 'Ann', 'Oslo'), (2, 'Bo', 'Rome');
insert into orders values (10, 1), (11, 2), (12, 1);
""")
selects = []
db.set_trace_callback(selects.append)


def select_customer(id):
    sql = "select name, city from customer where id = ?"
    return db.execute(sql, (id,)).fetchone()


class Customer:
    def __init__(self, id, name, city):
        self.id, self.name, self.city = id, name, city


# 1. Lazy initialization: a marker value (None) means "not loaded".
class Order:
    def __init__(self, id, customer_id):
        self.id, self.customer_id, self._customer = id, customer_id, None

    @property
    def customer(self):
        if self._customer is None:
            self._customer = Customer(self.customer_id, *select_customer(self.customer_id))
        return self._customer


# 2. Virtual proxy: it looks like a Customer and loads the real one on first use.
class CustomerProxy:
    def __init__(self, id):
        self.id, self._real = id, None

    def __getattr__(self, attr):  # Called only for attributes that the proxy lacks.
        if self._real is None:
            self._real = Customer(self.id, *select_customer(self.id))
        return getattr(self._real, attr)


# 3. Value holder: the client asks the holder for the value.
class ValueHolder:
    def __init__(self, loader):
        self._loader, self._value, self._loaded = loader, None, False

    def get_value(self):
        if not self._loaded:
            self._value, self._loaded = self._loader(), True
        return self._value


# 4. Ghost: the real class, with only the key filled in until first use.
class GhostCustomer:
    def __init__(self, id):
        self.id, self._loaded = id, False

    def _load(self):
        if not self._loaded:
            self._name, self._city = select_customer(self.id)
            self._loaded = True

    @property
    def name(self):
        self._load()
        return self._name

    @property
    def city(self):
        self._load()
        return self._city


selects.clear()
order = Order(10, 1)
line = f"lazy initialization: selects {len(selects)} | {order.customer.name}"
line += f" | selects {len(selects)} | {order.customer.name} | selects {len(selects)}"
print(line)

selects.clear()
proxy = CustomerProxy(2)
line = f"virtual proxy: id {proxy.id} | selects {len(selects)} | {proxy.name}"
print(line + f" | selects {len(selects)} | {proxy.city} | selects {len(selects)}")

selects.clear()
holder = ValueHolder(lambda: Customer(1, *select_customer(1)))
print(f"value holder: selects {len(selects)} | {holder.get_value().name} | selects {len(selects)}")

selects.clear()
ghost = GhostCustomer(1)
line = f"ghost: id {ghost.id} | selects {len(selects)} | {ghost.name} {ghost.city}"
print(line + f" | selects {len(selects)}")

selects.clear()
orders = [Order(*row) for row in db.execute("select id, customer_id from orders order by id")]
names = ", ".join(o.customer.name for o in orders)
print(f"lazy: {names} | selects {len(selects)}")

selects.clear()
sql = "select name from orders join customer on customer.id = orders.customer_id order by orders.id"
names = ", ".join(name for (name,) in db.execute(sql))
print(f"eager: {names} | selects {len(selects)}")
```

```output
lazy initialization: selects 0 | Ann | selects 1 | Ann | selects 1
virtual proxy: id 2 | selects 0 | Bo | selects 1 | Rome | selects 1
value holder: selects 0 | Ann | selects 1
ghost: id 1 | selects 0 | Ann Oslo | selects 1
lazy: Ann, Bo, Ann | selects 4
eager: Ann, Bo, Ann | selects 1
```

- Verdict: idiom. Lazy initialization is a `property` over a field that starts as `None`. The virtual proxy uses `__getattr__`, which Python calls only for attributes that the proxy does not have, so `proxy.id` needs no query and `proxy.name` loads the real customer.
- The file uses SQLite, and the trace callback counts the statements. `functools.cached_property` could replace the hand-written check in `Order.customer`.
