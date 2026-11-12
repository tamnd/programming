```python run id=name-the-choice
from enum import Enum


# Before: a flag argument chooses the operation.
def book_before(customer, is_premium):
    if is_premium:
        return {"customer": customer, "seat": "front", "fee": 40}
    return {"customer": customer, "seat": "general", "fee": 0}


# After, the callers pass literals: a function for each operation.
def premium_book(customer):
    return {"customer": customer, "seat": "front", "fee": 40}


def regular_book(customer):
    return {"customer": customer, "seat": "general", "fee": 0}


# After, the callers pass a value they read: a keyword-only enumeration.
class Tier(Enum):
    PREMIUM = "premium"
    REGULAR = "regular"


def book(customer, *, tier):
    if tier is Tier.PREMIUM:
        return premium_book(customer)
    if tier is Tier.REGULAR:
        return regular_book(customer)
    raise ValueError(f"unknown tier {tier!r}")


assert premium_book("ada") == book_before("ada", True)
assert regular_book("ada") == book_before("ada", False)
assert book("ada", tier=Tier("premium")) == book_before("ada", True)
try:
    book("ada", Tier.PREMIUM)  # the * makes tier keyword-only
    raise AssertionError("a positional tier should be refused")
except TypeError:
    pass
try:
    Tier("gold")
    raise AssertionError("an unknown tier should be refused")
except ValueError:
    pass
assert book_before("ada", "no")["fee"] == 40  # any truthy value counts as True
print("booking: tests pass")
```

```output
booking: tests pass
```

The `*` in `book(customer, *, tier)` makes `tier` keyword-only, so each call must write `tier=`, and `Tier("gold")` fails when the value is read, not later inside the function.
