```python run id=move-the-rules-in
import dataclasses
from dataclasses import dataclass
from datetime import date, timedelta


# Before: a data class, and the rules in the code that uses it.
class SubscriptionBefore:
    def __init__(self, start, end):
        self.start = start
        self.end = end


def billing_active(s, day):
    return s.start <= day < s.end


def reminder_days_left(s, day):
    return max(0, (s.end - day).days)


def renew_before(s, days):
    s.end = s.end + timedelta(days=days)


# After: a frozen dataclass that owns its rules.
@dataclass(frozen=True)
class Subscription:
    start: date
    end: date

    def __post_init__(self):
        if self.end <= self.start:
            raise ValueError("end must be after start")

    def is_active(self, day):
        return self.start <= day < self.end

    def days_left(self, day):
        return max(0, (self.end - day).days)

    def renewed(self, days):
        return dataclasses.replace(self, end=self.end + timedelta(days=days))


old = SubscriptionBefore(date(2026, 1, 1), date(2026, 2, 1))
sub = Subscription(date(2026, 1, 1), date(2026, 2, 1))
for day in [date(2025, 12, 31), date(2026, 1, 20), date(2026, 2, 1)]:
    assert sub.is_active(day) == billing_active(old, day)
    assert sub.days_left(day) == reminder_days_left(old, day)

renew_before(old, 30)
nxt = sub.renewed(30)
assert nxt.days_left(date(2026, 2, 1)) == reminder_days_left(old, date(2026, 2, 1)) == 30
assert sub.days_left(date(2026, 2, 1)) == 0
try:
    sub.end = date(2027, 1, 1)
    raise AssertionError("a frozen dataclass must not allow assignment")
except dataclasses.FrozenInstanceError:
    pass
try:
    Subscription(date(2026, 2, 1), date(2026, 1, 1))
    raise AssertionError("the rule must be checked")
except ValueError:
    pass
print("subscription: tests pass")
```

```output
subscription: tests pass
```

- `frozen=True` makes assignment to a field raise `FrozenInstanceError`, which removes the setters.
- `__post_init__` runs after the generated `__init__`, so the class can check its rule once, at construction.
- `dataclasses.replace` makes a copy with some fields changed, and runs `__post_init__` again on the copy.
