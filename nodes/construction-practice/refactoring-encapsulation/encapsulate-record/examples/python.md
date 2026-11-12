```python run id=hide-the-storage
import json
from dataclasses import FrozenInstanceError, asdict, dataclass

text = '{"sku": "T-100", "price_cents": 1999, "currency": "EUR"}'

# Before: a dictionary, read by key in each place.
raw = json.loads(text)
before = f"{raw['price_cents'] / 100:.2f} {raw['currency']}"

# After: a frozen dataclass. The fields are named, checked and read-only.
@dataclass(frozen=True)
class Product:
    sku: str
    price_cents: int
    currency: str

    def __post_init__(self):
        if self.price_cents < 0:
            raise ValueError(f"bad price {self.price_cents}")

    @property
    def price_text(self):  # derived, not stored
        return f"{self.price_cents / 100:.2f} {self.currency}"

p = Product(**json.loads(text))
assert p.price_text == before == "19.99 EUR"
try:
    p.price_cents = 0
    raise AssertionError("expected a frozen instance")
except FrozenInstanceError:
    pass
assert json.dumps(asdict(p)) == text  # back to the record form at the edge
print(p)
print("encapsulate record: tests pass")
```

```output
Product(sku='T-100', price_cents=1999, currency='EUR')
encapsulate record: tests pass
```
