```python run id=product-record
import sqlite3
from typing import Any, Self


class Record:
    """A base class for Active Records. Each subclass is one table, and each object is one row."""

    db: sqlite3.Connection
    table: str
    columns: tuple[str, ...]

    def __init__(self, id: int | None = None, **values: Any) -> None:
        self.id = id
        for column in self.columns:
            setattr(self, column, values[column])

    @classmethod
    def _load(cls, row: tuple) -> Self:
        return cls(row[0], **dict(zip(cls.columns, row[1:])))

    @classmethod
    def _select(cls) -> str:
        return f"SELECT id, {', '.join(cls.columns)} FROM {cls.table}"

    @classmethod
    def find(cls, id: int) -> Self | None:
        row = cls.db.execute(f"{cls._select()} WHERE id = ?", (id,)).fetchone()
        return cls._load(row) if row else None

    @classmethod
    def all(cls) -> list[Self]:
        return [cls._load(row) for row in cls.db.execute(f"{cls._select()} ORDER BY id")]

    def validate(self) -> None:
        """A subclass raises ValueError here when the object must not be saved."""

    def save(self) -> None:
        self.validate()
        values = [getattr(self, column) for column in self.columns]
        if self.id is None:
            marks = ", ".join("?" for _ in self.columns)
            self.id = self.db.execute(f"INSERT INTO {self.table} ({', '.join(self.columns)}) VALUES ({marks})", values).lastrowid
        else:
            sets = ", ".join(f"{column} = ?" for column in self.columns)
            self.db.execute(f"UPDATE {self.table} SET {sets} WHERE id = ?", [*values, self.id])

    def delete(self) -> None:
        self.db.execute(f"DELETE FROM {self.table} WHERE id = ?", (self.id,))


class Product(Record):
    """One row of the products table, with the domain logic of a product."""

    table = "products"
    columns = ("name", "price", "stock")

    def validate(self) -> None:
        if self.stock < 0:
            raise ValueError(f"{self.name}: stock cannot be negative")

    def needs_restock(self) -> bool:
        return self.stock < 5

    def sell(self, quantity: int) -> int:
        if quantity > self.stock:
            raise ValueError(f"{self.name}: only {self.stock} in stock")
        self.stock -= quantity
        return quantity * self.price

    def __str__(self) -> str:
        return f"{self.id} {self.name} {self.stock}"


Record.db = sqlite3.connect(":memory:")
Record.db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT NOT NULL, price INTEGER NOT NULL, stock INTEGER NOT NULL)")
for name, price, stock in [("pen", 120, 3), ("ink", 450, 40), ("pad", 300, 0)]:
    Product(name=name, price=price, stock=stock).save()

pen = Product.find(1)
print("sold 2 pens for", pen.sell(2))
pen.save()
restocked = []
for product in Product.all():
    if product.needs_restock():
        product.stock += 20
        product.save()
        restocked.append(product.name)
print("restocked:", ", ".join(restocked))
print("rows:", " | ".join(map(str, Product.all())))
ink = Product.find(2)
ink.stock -= 45
try:
    ink.save()
except ValueError as e:
    print("not saved:", e)
print("find(9):", Product.find(9) or "none")
```

```output
sold 2 pens for 240
restocked: pen, pad
rows: 1 pen 21 | 2 ink 40 | 3 pad 20
not saved: ink: stock cannot be negative
find(9): none
```

- Verdict: idiom. A small `Record` base class holds the finders and `save`, and `Product` adds the columns, the validation and the domain methods, as a Rails or Django model does.
- The SQL is built from the column list of the subclass. Only column names from the class go into the SQL text, and all values are `?` parameters.
