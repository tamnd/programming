```python run id=name-the-group
from dataclasses import FrozenInstanceError, dataclass

meetings = [("standup", 8 * 60), ("review", 10 * 60), ("planning", 16 * 60), ("retro", 18 * 60)]

# Before: the pair (open_, close) travels as two parameters.
def outside_hours_before(items, open_, close):
    return [t for t, start in items if start < open_ or start >= close]

# After: a frozen dataclass is a value with a rule and a behaviour.
@dataclass(frozen=True)
class TimeWindow:
    open: int
    close: int

    def __post_init__(self):
        if self.open >= self.close:
            raise ValueError(f"open {self.open} is not before close {self.close}")

    def includes(self, minute):
        return self.open <= minute < self.close

def outside_hours(items, hours):
    return [t for t, start in items if not hours.includes(start)]

office = TimeWindow(9 * 60, 17 * 60)
assert outside_hours(meetings, office) == outside_hours_before(meetings, 9 * 60, 17 * 60)
assert outside_hours(meetings, office) == ["standup", "retro"]

for bad in (lambda: TimeWindow(17 * 60, 9 * 60), lambda: setattr(office, "open", 0)):
    try:
        bad()
        raise AssertionError("expected an error")
    except (ValueError, FrozenInstanceError) as e:
        print(type(e).__name__)
print("introduce parameter object: tests pass")
```

```output
ValueError
FrozenInstanceError
introduce parameter object: tests pass
```
