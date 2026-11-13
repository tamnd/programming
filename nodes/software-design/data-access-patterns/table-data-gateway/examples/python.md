```python run id=products-gateway
import sqlite3


class ProductGateway:
    """One instance handles every row of the products table."""

    def __init__(self, db: sqlite3.Connection) -> None:
        self._db = db

    def find(self, product_id: int) -> sqlite3.Row | None:
        return self._db.execute("SELECT id, name, price, stock FROM products WHERE id = ?", (product_id,)).fetchone()

    def find_all(self) -> list[sqlite3.Row]:
        return self._db.execute("SELECT id, name, price, stock FROM products ORDER BY id").fetchall()

    def find_low_stock(self, limit: int) -> list[sqlite3.Row]:
        return self._db.execute("SELECT id, name, price, stock FROM products WHERE stock < ? ORDER BY id", (limit,)).fetchall()

    def insert(self, name: str, price: int, stock: int) -> int:
        return self._db.execute("INSERT INTO products (name, price, stock) VALUES (?, ?, ?)", (name, price, stock)).lastrowid

    def update_stock(self, product_id: int, stock: int) -> None:
        self._db.execute("UPDATE products SET stock = ? WHERE id = ?", (stock, product_id))

    def delete(self, product_id: int) -> None:
        self._db.execute("DELETE FROM products WHERE id = ?", (product_id,))


def restock(products: ProductGateway, limit: int, amount: int) -> list[str]:
    done = []
    for row in products.find_low_stock(limit):
        products.update_stock(row["id"], row["stock"] + amount)
        done.append(f"{row['name']} {row['stock']} -> {row['stock'] + amount}")
    return done


db = sqlite3.connect(":memory:")
db.row_factory = sqlite3.Row
db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT NOT NULL, price INTEGER NOT NULL, stock INTEGER NOT NULL)")
products = ProductGateway(db)
products.insert("pen", 120, 3)
ink = products.insert("ink", 450, 40)
products.insert("pad", 300, 0)
with db:
    print("restocked:", "; ".join(restock(products, 5, 20)))
products.delete(ink)
print("rows:", " | ".join(f"{r['id']} {r['name']} {r['price']} {r['stock']}" for r in products.find_all()))
print("find(9):", products.find(9) or "none")
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. The gateway runs real SQL on SQLite, and every value is bound with a `?` placeholder.
- The finders return `sqlite3.Row` values, so the caller reads columns by name and the gateway needs no row class.
- `with db:` makes the restock run one transaction: it commits when the block ends, and it rolls back on an exception.
