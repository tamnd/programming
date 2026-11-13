```python run id=product-rows
import sqlite3


class ProductRow:
    """A gateway to one row: the columns are fields, and the row saves itself."""

    def __init__(self, db: sqlite3.Connection, name: str, price: int, stock: int, id: int | None = None) -> None:
        self._db = db
        self.id, self.name, self.price, self.stock = id, name, price, stock

    def insert(self) -> int:
        self.id = self._db.execute("INSERT INTO products (name, price, stock) VALUES (?, ?, ?)", (self.name, self.price, self.stock)).lastrowid
        return self.id

    def update(self) -> None:
        self._db.execute("UPDATE products SET name = ?, price = ?, stock = ? WHERE id = ?", (self.name, self.price, self.stock, self.id))

    def delete(self) -> None:
        self._db.execute("DELETE FROM products WHERE id = ?", (self.id,))

    def __str__(self) -> str:
        return f"{self.id} {self.name} {self.price} {self.stock}"


class ProductFinder:
    """Reads rows and makes one ProductRow for each of them."""

    COLUMNS = "SELECT id, name, price, stock FROM products"

    def __init__(self, db: sqlite3.Connection) -> None:
        self._db = db

    def _load(self, row: tuple) -> ProductRow:
        id, name, price, stock = row
        return ProductRow(self._db, name, price, stock, id)

    def find(self, product_id: int) -> ProductRow | None:
        row = self._db.execute(self.COLUMNS + " WHERE id = ?", (product_id,)).fetchone()
        return self._load(row) if row else None

    def find_all(self) -> list[ProductRow]:
        return [self._load(r) for r in self._db.execute(self.COLUMNS + " ORDER BY id")]

    def find_low_stock(self, limit: int) -> list[ProductRow]:
        return [self._load(r) for r in self._db.execute(self.COLUMNS + " WHERE stock < ? ORDER BY id", (limit,))]


def restock(finder: ProductFinder, limit: int, amount: int) -> list[str]:
    done = []
    for product in finder.find_low_stock(limit):
        before = product.stock
        product.stock += amount
        product.update()
        done.append(f"{product.name} {before} -> {product.stock}")
    return done


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT NOT NULL, price INTEGER NOT NULL, stock INTEGER NOT NULL)")
ProductRow(db, "pen", 120, 3).insert()
ink = ProductRow(db, "ink", 450, 40).insert()
ProductRow(db, "pad", 300, 0).insert()
finder = ProductFinder(db)
with db:
    print("restocked:", "; ".join(restock(finder, 5, 20)))
finder.find(ink).delete()
print("rows:", " | ".join(str(p) for p in finder.find_all()))
print("find(9):", finder.find(9) or "none")
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. A plain class with one attribute per column and `insert`, `update` and `delete` methods that run SQL on SQLite.
- The finder is a separate class. It holds the read queries, and it makes one `ProductRow` for each row that it reads.
