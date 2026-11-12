```python run id=write-it-once
from datetime import date, timedelta


# Before: each client writes the missing operation inline.
def due_date_before(invoice_date):
    d = invoice_date + timedelta(days=1)
    while d.weekday() >= 5:
        d += timedelta(days=1)
    return d


def ship_date_before(order_date):
    d = order_date + timedelta(days=1)
    while d.weekday() == 6:  # forgot Saturday
        d += timedelta(days=1)
    return d


# After, one or two operations: a foreign function that takes the library object first.
# It belongs on datetime.date, which the program cannot change.
def next_workday(day):
    d = day + timedelta(days=1)
    while d.weekday() >= 5:
        d += timedelta(days=1)
    return d


# After, many operations: a local extension as a subclass.
class Workday(date):
    @classmethod
    def of(cls, day):
        return cls(day.year, day.month, day.day)

    def is_workday(self):
        return self.weekday() < 5

    def next_workday(self):
        return Workday.of(next_workday(self))


cases = [
    (date(2026, 11, 13), date(2026, 11, 16)),
    (date(2026, 11, 14), date(2026, 11, 16)),
    (date(2026, 11, 17), date(2026, 11, 18)),
]
for start, want in cases:
    assert next_workday(start) == want
    assert next_workday(start) == due_date_before(start)
    assert Workday.of(start).next_workday() == want
assert ship_date_before(date(2026, 11, 13)) == date(2026, 11, 14)  # a Saturday

# The local extension is a date, so date code accepts it, but a date
# that other code makes is a plain date, and must be converted.
w = Workday.of(date(2026, 11, 13))
assert isinstance(w, date) and w.is_workday()
parsed = date.fromisoformat("2026-11-14")
assert not hasattr(parsed, "is_workday")
assert not Workday.of(parsed).is_workday()
print("next_workday: tests pass")
```

```output
next_workday: tests pass
```

The subclass shows the cost of a local extension. A `Workday` is a `date`, so the library accepts it, but a date that the library or other code makes, such as the result of `date.fromisoformat`, is a plain `date`, and the program must convert it with `Workday.of` before it can call the extra methods.
