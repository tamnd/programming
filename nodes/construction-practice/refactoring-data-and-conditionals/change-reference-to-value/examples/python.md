```python run id=replace-not-change
import dataclasses
from dataclasses import dataclass


@dataclass(frozen=True)
class DeliveryWindow:
    day: int
    start_hour: int
    end_hour: int

    def moved_by(self, days: int) -> "DeliveryWindow":
        return dataclasses.replace(self, day=self.day + days)


morning = DeliveryWindow(3, 9, 12)
c = {"id": "c", "window": morning}
d = {"id": "d", "window": morning}
c["window"] = c["window"].moved_by(1)
assert d["window"].day == 3                      # d is not moved
assert c["window"] == DeliveryWindow(4, 9, 12)   # equality from the fields
assert len({DeliveryWindow(4, 9, 12), c["window"]}) == 1  # hash agrees with equality
try:
    morning.day = 5
    raise RuntimeError("expected FrozenInstanceError")
except dataclasses.FrozenInstanceError:
    pass


# Freezing is shallow: a list in a frozen dataclass can still change.
@dataclass(frozen=True)
class Route:
    stops: list[str]


r = Route(["depot"])
r.stops.append("harbour")
assert r.stops == ["depot", "harbour"]
print("change reference to value: tests pass")
```

```output
change reference to value: tests pass
```
