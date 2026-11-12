```python run id=name-the-number
import math
from typing import Final


def parcel_cost_before(grams: int, zone: str) -> int:
    cents = 450
    if grams > 2000:
        cents += math.ceil((grams - 2000) / 1000) * 120
    if zone == "remote":
        cents += 300
    return round(cents * 1.25)


BASE_FEE_CENTS: Final = 450
FREE_WEIGHT_GRAMS: Final = 2000
GRAMS_PER_KILOGRAM: Final = 1000
FEE_PER_EXTRA_KILOGRAM_CENTS: Final = 120
REMOTE_ZONE: Final = "remote"
REMOTE_FEE_CENTS: Final = 300
TAX_FACTOR: Final = 1.25  # 25 percent tax


def parcel_cost(grams: int, zone: str) -> int:
    cents = BASE_FEE_CENTS
    if grams > FREE_WEIGHT_GRAMS:
        extra_kilograms = math.ceil((grams - FREE_WEIGHT_GRAMS) / GRAMS_PER_KILOGRAM)
        cents += extra_kilograms * FEE_PER_EXTRA_KILOGRAM_CENTS
    if zone == REMOTE_ZONE:
        cents += REMOTE_FEE_CENTS
    return round(cents * TAX_FACTOR)


for g in [0, 2000, 2001, 3001, 10000]:
    for z in ["city", "remote"]:
        assert parcel_cost(g, z) == parcel_cost_before(g, z)
print("replace magic literal: tests pass")
```

```output
replace magic literal: tests pass
```
