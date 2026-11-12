```python run id=money-type
import re
from dataclasses import dataclass
from functools import reduce
from typing import NewType


# Before: money is a number of cents and a separate string.
def total_before(lines):
    return sum(line["cents"] for line in lines)


def format_before(cents, currency):
    return f"{cents / 100:.2f} {currency}"


lines = [
    {"cents": 1250, "currency": "EUR"},
    {"cents": 800, "currency": "EUR"},
    {"cents": 500, "currency": "USD"},
]
assert format_before(total_before(lines), "EUR") == "25.50 EUR"  # 5 dollars counted as 5 euros
print("before: the total mixes two currencies")


# After: Money holds the amount, the currency and the rules.
@dataclass(frozen=True)
class Money:
    cents: int
    currency: str

    def __post_init__(self):
        if not isinstance(self.cents, int):
            raise TypeError(f"cents must be an integer: {self.cents!r}")
        if not re.fullmatch(r"[A-Z]{3}", self.currency):
            raise ValueError(f"currency code {self.currency!r}")

    def __add__(self, other):
        if other.currency != self.currency:
            raise ValueError(f"cannot add {other.currency} to {self.currency}")
        return Money(self.cents + other.cents, self.currency)

    def __str__(self):
        return f"{self.cents / 100:.2f} {self.currency}"


def total(prices):
    return reduce(lambda a, b: a + b, prices)


def raises(exc, f):
    try:
        f()
    except exc:
        return True
    return False


euros = [Money(1250, "EUR"), Money(800, "EUR")]
assert str(total(euros)) == "20.50 EUR"
assert raises(ValueError, lambda: total([*euros, Money(500, "USD")]))
assert raises(TypeError, lambda: Money(12.5, "EUR"))
assert raises(ValueError, lambda: Money(100, "euro"))

# NewType: a distinct type for the type checker, a plain int at runtime.
UserId = NewType("UserId", int)
uid = UserId(42)
assert uid == 42 and type(uid) is int
print("after: tests pass")
```

```output
before: the total mixes two currencies
after: tests pass
```

- `__add__` lets the code write `a + b` for two amounts, and the currency check runs on each addition.
- `NewType` costs nothing at runtime, as the last test shows: `UserId(42)` is the integer 42. A type checker such as mypy reports a call that passes a plain `int` where a `UserId` is expected, but Python itself does not.
