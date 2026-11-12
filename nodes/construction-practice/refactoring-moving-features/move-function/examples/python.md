```python run id=near-its-data
import math
from dataclasses import dataclass


@dataclass(frozen=True)
class Parcel:
    weight_kg: float
    cm: tuple[int, int, int]

    @property
    def shipping_cents(self) -> int:
        # Moved here from Order: it uses only the parcel's data.
        volumetric_kg = self.cm[0] * self.cm[1] * self.cm[2] / 5000
        return 400 + math.ceil(max(self.weight_kg, volumetric_kg)) * 150


@dataclass(frozen=True)
class Order:
    order_id: int
    parcel: Parcel

    @property
    def shipping_cents(self) -> int:
        return self.parcel.shipping_cents  # delegates while callers change


assert Order(1, Parcel(0.8, (10, 10, 5))).shipping_cents == 550
assert Order(2, Parcel(1.2, (60, 40, 40))).shipping_cents == 3400
print("move function: tests pass")
```

```output
move function: tests pass
```
