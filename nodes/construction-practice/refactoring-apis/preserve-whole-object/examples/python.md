```python run id=whole-object
from dataclasses import dataclass


@dataclass(frozen=True)
class TempRange:
    low: int
    high: int


@dataclass(frozen=True)
class Plan:
    low: int
    high: int


def within_range_before(plan: Plan, bottom: int, top: int) -> bool:
    return bottom >= plan.low and top <= plan.high


def within_range(plan: Plan, temp_range: TempRange) -> bool:
    return temp_range.low >= plan.low and temp_range.high <= plan.high


days_temp_range = TempRange(18, 23)
plan = Plan(16, 24)
assert within_range(plan, days_temp_range) == within_range_before(plan, days_temp_range.low, days_temp_range.high)
assert not within_range(plan, TempRange(12, 20))
print(within_range(plan, days_temp_range))
```

```output
True
```
