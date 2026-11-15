```python run id=shop-on-two-servers
import sqlite3

TIMEOUT = 30  # minutes with no request before a session ends
URI = "file:shop?mode=memory&cache=shared"


def connect():
    # Each server opens its own connection to the same database.
    return sqlite3.connect(URI, uri=True, isolation_level=None)


def setup(db):
    db.executescript("""
        CREATE TABLE sessions (id TEXT PRIMARY KEY, user TEXT, last_used INTEGER);
        CREATE TABLE cart_lines (session_id TEXT, item TEXT, qty INTEGER,
                                 PRIMARY KEY (session_id, item));
        CREATE TABLE orders (id INTEGER PRIMARY KEY, user TEXT);
        CREATE TABLE order_lines (order_id INTEGER, item TEXT, qty INTEGER);
    """)


def hhmm(t):
    return f"{t // 60}:{t % 60:02d}"


def start(db, sid, user, now):
    db.execute("INSERT INTO sessions VALUES (?, ?, ?)", (sid, user, now))


def touch(db, sid, now):
    # Return the user of the session, or None if it is unknown or idle.
    row = db.execute("SELECT user, last_used FROM sessions WHERE id = ?", (sid,)).fetchone()
    if row is None or now - row[1] > TIMEOUT:
        return None
    db.execute("UPDATE sessions SET last_used = ? WHERE id = ?", (now, sid))
    return row[0]


def cart(db, sid):
    rows = db.execute(
        "SELECT item, qty FROM cart_lines WHERE session_id = ? ORDER BY rowid", (sid,))
    return ", ".join(f"{item} {qty}" for item, qty in rows)


def add(db, server, sid, item, now):
    user = touch(db, sid, now)
    db.execute("""INSERT INTO cart_lines VALUES (?, ?, 1)
                  ON CONFLICT (session_id, item) DO UPDATE SET qty = qty + 1""", (sid, item))
    print(f"{hhmm(now)} {user} adds {item} on server {server}: cart {cart(db, sid)}")


def checkout(db, server, sid, now):
    user = touch(db, sid, now)
    lines = cart(db, sid)
    # The pending cart lines become an order in one transaction.
    db.execute("BEGIN")
    oid = db.execute("INSERT INTO orders (user) VALUES (?)", (user,)).lastrowid
    db.execute("""INSERT INTO order_lines SELECT ?, item, qty FROM cart_lines
                  WHERE session_id = ? ORDER BY rowid""", (oid, sid))
    db.execute("DELETE FROM cart_lines WHERE session_id = ?", (sid,))
    db.execute("COMMIT")
    print(f"{hhmm(now)} {user} checks out on server {server}: order {oid} with {lines}")


def report(db):
    rows = db.execute("SELECT item, SUM(qty) FROM order_lines GROUP BY item ORDER BY item")
    print("sales:", ", ".join(f"{item} {qty}" for item, qty in rows))


def purge(db, now):
    idle = "SELECT id FROM sessions WHERE ? - last_used > ?"
    ids = [r[0] for r in db.execute(idle + " ORDER BY id", (now, TIMEOUT))]
    db.execute("BEGIN")
    db.execute(f"DELETE FROM cart_lines WHERE session_id IN ({idle})", (now, TIMEOUT))
    db.execute("DELETE FROM sessions WHERE ? - last_used > ?", (now, TIMEOUT))
    db.execute("COMMIT")
    print(f"{hhmm(now)} purge sessions idle for more than {TIMEOUT} minutes: {', '.join(ids)}")


def count(db, table):
    return db.execute(f"SELECT COUNT(*) FROM {table}").fetchone()[0]


a, b = connect(), connect()
setup(a)
start(a, "s1", "ann", 600)
start(a, "s2", "bob", 601)
add(a, "a", "s1", "pen", 600)
add(a, "a", "s2", "ink", 601)
add(b, "b", "s1", "pen", 603)  # server b reads the rows that server a wrote
add(a, "a", "s1", "ink", 604)
checkout(b, "b", "s1", 625)
report(a)
purge(a, 640)
print(f"rows left: sessions {count(a, 'sessions')}, cart lines {count(a, 'cart_lines')}, "
      f"orders {count(a, 'orders')}, order lines {count(a, 'order_lines')}")
```

```output
10:00 ann adds pen on server a: cart pen 1
10:01 bob adds ink on server a: cart ink 1
10:03 ann adds pen on server b: cart pen 2
10:04 ann adds ink on server a: cart pen 2, ink 1
10:25 ann checks out on server b: order 1 with pen 2, ink 1
sales: ink 1, pen 2
10:40 purge sessions idle for more than 30 minutes: s2
rows left: sessions 1, cart lines 0, orders 1, order lines 2
```

- Verdict: stdlib. The `sqlite3` module is part of the Python standard library. The two connections `a` and `b` stand for two servers that share one database. The URI `file:shop?mode=memory&cache=shared` keeps the database in memory, so the example writes no file.
- The session data is in its own tables, `sessions` and `cart_lines`. The checkout moves the pending lines into `orders` and `order_lines` in one transaction, so the sales report never sees a cart that is not an order.
- `purge` removes the sessions that are idle for more than `TIMEOUT` minutes, with their cart lines. A real server runs it on a schedule.
