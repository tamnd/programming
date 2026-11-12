```python run id=date-range
from dataclasses import dataclass


# Before: each period is two loose numbers.
def overlaps_before(a_start, a_end, b_start, b_end):
    return a_start <= b_end and b_start <= a_end


def contains_before(start, end, day):
    return start <= day <= end


assert overlaps_before(1, 5, 4, 9)
assert not overlaps_before(1, 4, 5, 9)
assert not overlaps_before(1, 5, 9, 4)  # the second period is reversed, and no one is told
print("before: a reversed period gives a wrong answer")


# After: the clump is a type, and the rules live in it.
@dataclass(frozen=True)
class DateRange:
    start: int
    end: int

    def __post_init__(self):
        if self.end < self.start:
            raise ValueError(f"range {self.start}..{self.end} ends before it starts")

    def contains(self, day):
        return self.start <= day <= self.end

    def overlaps(self, other):
        return self.start <= other.end and other.start <= self.end


week = DateRange(1, 5)
assert week.overlaps(DateRange(4, 9))
assert not week.overlaps(DateRange(6, 9))
assert week.contains(5)
assert not week.contains(6)
assert contains_before(1, 5, 5) == week.contains(5)
assert week == DateRange(1, 5)
try:
    DateRange(9, 4)
except ValueError:
    pass
else:
    raise AssertionError("a reversed range was accepted")
print("after: tests pass")
```

```output
before: a reversed period gives a wrong answer
after: tests pass
```

- `frozen=True` makes the fields read-only after construction, so a range that passed the check cannot be changed into one that fails it.
- The dataclass writes `__eq__` from the fields, so two ranges with the same start and end are equal. That is the behaviour of a value.
