```python run id=one-home
import math
from functools import cached_property

DIVISOR = 5000  # cm^3 per kg of volumetric weight

# Before: functions that each take the parcel record.
def chargeable_weight(p):
    volume = p["length"] * p["width"] * p["height"]
    return max(p["weight_kg"], volume / DIVISOR)

def shipping_cost(p, rate_per_kg):
    return math.ceil(chargeable_weight(p)) * rate_per_kg

# After: one class. The parcel does not change, so a derived value
# can be computed once and kept with cached_property.
class Parcel:
    def __init__(self, length, width, height, weight_kg):
        self._dims = (length, width, height)
        self._weight_kg = weight_kg

    @cached_property
    def volume(self):
        return math.prod(self._dims)

    @property
    def chargeable_weight(self):
        return max(self._weight_kg, self.volume / DIVISOR)

    def shipping_cost(self, rate_per_kg):
        return math.ceil(self.chargeable_weight) * rate_per_kg

records = [
    {"length": 40, "width": 30, "height": 20, "weight_kg": 2},
    {"length": 10, "width": 10, "height": 10, "weight_kg": 7},
]
costs = [Parcel(**r).shipping_cost(3) for r in records]
assert costs == [shipping_cost(r, 3) for r in records] == [15, 21]
print(costs)
print("combine functions into class: tests pass")
```

```output
[15, 21]
combine functions into class: tests pass
```
