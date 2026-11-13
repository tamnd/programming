```python run id=product-mapper
import sqlite3
from dataclasses import dataclass, field


@dataclass
class Product:
    """A domain object. It has no SQL and no connection."""

    name: str
    price: int
    stock: int
    tags: list[str] = field(default_factory=list)
    id: int | None = None

    def sell(self, quantity: int) -> int:
        if quantity > self.stock:
            raise ValueError(f"{self.name}: only {self.stock} in stock")
        self.stock -= quantity
        return quantity * self.price

    def __str__(self) -> str:
        return f"{self.id} {self.name} {self.price} {self.stock} [{', '.join(self.tags)}]"


class ProductMapper:
    """Moves products between objects and two tables: products, and product_tags for the tags list."""

    def __init__(self, db: sqlite3.Connection) -> None:
        self._db = db

    def find(self, product_id: int) -> Product | None:
        row = self._db.execute("SELECT id, title, price_cents, qty FROM products WHERE id = ?", (product_id,)).fetchone()
        if row is None:
            return None
        tags = [t for (t,) in self._db.execute("SELECT tag FROM product_tags WHERE product_id = ? ORDER BY tag", (product_id,))]
        return Product(name=row[1], price=row[2], stock=row[3], tags=tags, id=row[0])

    def insert(self, p: Product) -> None:
        p.id = self._db.execute("INSERT INTO products (title, price_cents, qty) VALUES (?, ?, ?)", (p.name, p.price, p.stock)).lastrowid
        self._write_tags(p)

    def update(self, p: Product) -> None:
        self._db.execute("UPDATE products SET title = ?, price_cents = ?, qty = ? WHERE id = ?", (p.name, p.price, p.stock, p.id))
        self._db.execute("DELETE FROM product_tags WHERE product_id = ?", (p.id,))
        self._write_tags(p)

    def delete(self, p: Product) -> None:
        self._db.execute("DELETE FROM product_tags WHERE product_id = ?", (p.id,))
        self._db.execute("DELETE FROM products WHERE id = ?", (p.id,))

    def _write_tags(self, p: Product) -> None:
        self._db.executemany("INSERT INTO product_tags (product_id, tag) VALUES (?, ?)", [(p.id, t) for t in p.tags])


db = sqlite3.connect(":memory:")
db.executescript("""
CREATE TABLE products (id INTEGER PRIMARY KEY, title TEXT NOT NULL, price_cents INTEGER NOT NULL, qty INTEGER NOT NULL);
CREATE TABLE product_tags (product_id INTEGER NOT NULL REFERENCES products(id), tag TEXT NOT NULL);
""")
mapper = ProductMapper(db)
mapper.insert(Product("pen", 120, 3, ["office"]))
ink = Product("ink", 450, 40, ["office"])
mapper.insert(ink)

pen = mapper.find(1)
print("sold:", pen.sell(2))
pen.tags.append("sale")
mapper.update(pen)
print("object:", mapper.find(1))
print("products:", " | ".join(" ".join(map(str, r)) for r in db.execute("SELECT * FROM products ORDER BY id")))
print("product_tags:", " | ".join(f"{i} {t}" for i, t in db.execute("SELECT * FROM product_tags ORDER BY product_id, tag")))
mapper.delete(ink)
products, tags = (db.execute(f"SELECT count(*) FROM {t}").fetchone()[0] for t in ("products", "product_tags"))
print(f"after delete: {products} product, {tags} tags")
print("find(2):", mapper.find(2) or "none")
```

```output
sold: 240
object: 1 pen 120 1 [office, sale]
products: 1 pen 120 1 | 2 ink 450 40
product_tags: 1 office | 1 sale | 2 office
after delete: 1 product, 2 tags
find(2): none
```

- Verdict: idiom. `Product` is a plain dataclass with a domain method, and `ProductMapper` holds all the SQL for the two tables.
- The column names (`title`, `price_cents`, `qty`) differ from the field names, and the `tags` list is stored as rows of a second table. Only the mapper knows this.
