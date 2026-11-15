```python run id=date-range-value
from dataclasses import dataclass, replace
from datetime import date


@dataclass(frozen=True)  # equality and hash by the fields; no assignment after creation
class DateRange:
    start: date
    end: date

    def __post_init__(self):
        if self.end < self.start:
            raise ValueError("the end is before the start")

    def contains(self, day):
        return self.start <= day <= self.end

    def extend_to(self, end):
        return replace(self, end=end)  # a new value; self does not change

    def __str__(self):
        return f"{self.start}..{self.end}"


def yn(b):
    return "yes" if b else "no"


a = DateRange(date(2026, 3, 1), date(2026, 3, 10))
b = DateRange(date(2026, 3, 1), date(2026, 3, 10))
print("a equals b:", yn(a == b))
print("a and b are the same object:", yn(a is b))

c = a.extend_to(date(2026, 3, 15))
print("a:", a)
print("c:", c)

rooms = {a: "room 4"}
print("the room for b:", rooms[b])
print("a contains 2026-03-05:", yn(a.contains(date(2026, 3, 5))))
print("a contains 2026-03-12:", yn(a.contains(date(2026, 3, 12))))

try:
    DateRange(date(2026, 3, 10), date(2026, 3, 1))
except ValueError as e:
    print("error:", e)
```

```output
a equals b: yes
a and b are the same object: no
a: 2026-03-01..2026-03-10
c: 2026-03-01..2026-03-15
the room for b: room 4
a contains 2026-03-05: yes
a contains 2026-03-12: no
error: the end is before the start
```

- Verdict: stdlib. `@dataclass(frozen=True)` generates `__eq__` and `__hash__` from the fields, and an assignment to a field raises `FrozenInstanceError`. `dataclasses.replace` makes a changed copy.
- `__post_init__` checks the rule of the value, so no `DateRange` can have its end before its start.
