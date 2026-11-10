```python wrong id=concatenation
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE orders (id INTEGER, customer TEXT, total REAL)")
db.executemany("INSERT INTO orders VALUES (?, ?, ?)", [(1, "alice", 12.5), (2, "bob", 40.0)])


def find_orders(customer):
    query = "SELECT id, customer, total FROM orders WHERE customer = '" + customer + "'"
    print(query)
    return db.execute(query).fetchall()


print(find_orders("alice"))
print(find_orders("x' OR '1'='1"))
```

```output for=concatenation
SELECT id, customer, total FROM orders WHERE customer = 'alice'
[(1, 'alice', 12.5)]
SELECT id, customer, total FROM orders WHERE customer = 'x' OR '1'='1'
[(1, 'alice', 12.5), (2, 'bob', 40.0)]
```

The second call returns every row. The quote in the input closes the string literal, and the rest of the input becomes part of the `WHERE` clause.

```python run id=placeholder
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE orders (id INTEGER, customer TEXT, total REAL)")
db.executemany("INSERT INTO orders VALUES (?, ?, ?)", [(1, "alice", 12.5), (2, "bob", 40.0)])


def find_orders(customer):
    query = "SELECT id, customer, total FROM orders WHERE customer = ?"
    return db.execute(query, (customer,)).fetchall()


print(find_orders("alice"))
print(find_orders("x' OR '1'='1"))
print(find_orders("O'Brien"))

SORT_COLUMNS = {"id": "id", "total": "total"}


def list_orders(sort):
    column = SORT_COLUMNS.get(sort)
    if column is None:
        raise ValueError(f"cannot sort by {sort!r}")
    return db.execute(f"SELECT id FROM orders ORDER BY {column} DESC").fetchall()


print(list_orders("total"))
try:
    list_orders("password")
except ValueError as error:
    print(error)
```

```output for=placeholder
[(1, 'alice', 12.5)]
[]
[]
[(2,), (1,)]
cannot sort by 'password'
```

```python run id=second-order
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE users (id INTEGER, name TEXT)")
db.execute("CREATE TABLE audit (name TEXT, action TEXT)")

# Step 1: the sign-up code binds the name, so the insert is safe.
db.execute("INSERT INTO users VALUES (?, ?)", (1, "O'Brien"))

# Step 2: a later job trusts the stored name and builds SQL from it.
(name,) = db.execute("SELECT name FROM users WHERE id = 1").fetchone()
try:
    db.execute("INSERT INTO audit VALUES ('" + name + "', 'login')")
except sqlite3.OperationalError as error:
    print("unsafe:", error)

# The fix: bind the stored value too.
db.execute("INSERT INTO audit VALUES (?, ?)", (name, "login"))
print(db.execute("SELECT * FROM audit").fetchall())
```

```output for=second-order
unsafe: near "Brien": syntax error
[("O'Brien", 'login')]
```

- Verdict: stdlib. The module `sqlite3` binds values to the placeholders `?` and `:name`. Other drivers that follow the database API of PEP 249 use their own placeholder style, such as `%s` in psycopg.
- `Connection.execute` runs one statement and raises `ProgrammingError` on text with two statements. `executescript` runs many statements and takes no parameters, so never build its text from input.
- A placeholder holds a value, not a name. A column name in `ORDER BY` comes from a fixed dictionary, as in `list_orders`.
- The second-order example uses the harmless name `O'Brien`. The stored value breaks the second query, which shows that the same input could also change it. Values that come from the database need the same binding as values from a request.
- Ruff rule S608, which comes from flake8-bandit, flags the query built with `+` in the first example. It also flags the f-string in `list_orders`, where the allow-list makes it safe. Mark such a line with `# noqa: S608` and a comment that names the allow-list.
