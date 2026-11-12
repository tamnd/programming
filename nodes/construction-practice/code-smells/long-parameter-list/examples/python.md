```python run id=parameter-object
from dataclasses import dataclass, replace


# Before: six positional parameters.
def stay_price_before(start_day, end_day, nightly_rate, breakfast_rate, guests, with_breakfast):
    nights = end_day - start_day
    price = nights * nightly_rate
    if with_breakfast:
        price += nights * guests * breakfast_rate
    return price


# After: a date range, the room as a whole, and a stay that holds the rest.
@dataclass(frozen=True)
class DateRange:
    start: int
    end: int

    def nights(self):
        return self.end - self.start


@dataclass(frozen=True)
class Room:
    nightly_rate: int
    breakfast_rate: int


@dataclass(frozen=True)
class Stay:
    range: DateRange
    guests: int
    breakfast: bool


def stay_price(stay, room):
    n = stay.range.nights()
    breakfast = n * stay.guests * room.breakfast_rate if stay.breakfast else 0
    return n * room.nightly_rate + breakfast


# A lighter fix: keyword-only parameters, so that each call names its values.
def stay_price_kw(*, start_day, end_day, nightly_rate, breakfast_rate, guests, with_breakfast):
    return stay_price_before(start_day, end_day, nightly_rate, breakfast_rate, guests, with_breakfast)


assert stay_price_before(10, 13, 8000, 1500, 2, True) == 33000
assert stay_price_before(10, 13, 1500, 8000, 2, True) == 52500  # swapped, and no error
print("stay_price_before: tests pass")

room = Room(nightly_rate=8000, breakfast_rate=1500)
stay = Stay(DateRange(10, 13), guests=2, breakfast=True)
assert stay_price(stay, room) == 33000
assert stay_price(replace(stay, breakfast=False), room) == 24000
print("stay_price: tests pass")

assert stay_price_kw(start_day=10, end_day=13, nightly_rate=8000,
                     breakfast_rate=1500, guests=2, with_breakfast=True) == 33000
try:
    stay_price_kw(10, 13, 8000, 1500, 2, True)
except TypeError:
    print("stay_price_kw: positional call rejected")
else:
    raise AssertionError("positional call accepted")
```

```output
stay_price_before: tests pass
stay_price: tests pass
stay_price_kw: positional call rejected
```

- `DateRange.nights` shows the next step after Introduce Parameter Object: the behaviour that belongs to the range moves into it.
- `dataclasses.replace` makes a copy of a frozen dataclass with one field changed, which the test uses to turn breakfast off.
- The keyword-only version keeps the six parameters, but a call with the values in the wrong order cannot be written by accident, because each value has its name next to it.
