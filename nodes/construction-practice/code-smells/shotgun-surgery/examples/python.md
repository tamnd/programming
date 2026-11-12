```python run id=gather-the-codes
# Before: three functions each know the codes.
def price_before(cents, code):
    if code == "SPRING10":
        return round(cents * 0.9)
    if code == "VIP25":
        return round(cents * 0.75)
    return cents


def receipt_before(code):
    if code == "SPRING10":
        return "SPRING10 (10% off)"
    if code == "VIP25":
        return "VIP25 (25% off)"
    return "no discount"


def is_valid_before(code):
    return code in ("SPRING10", "VIP25", "SUMMER20")
# SUMMER20 was added to the validation, but not to the price or the receipt.


assert is_valid_before("SUMMER20")
assert price_before(1000, "SUMMER20") == 1000  # a valid code with no effect
print("before: one of three edits was made")

# After: one table owns the codes, and the three functions read it.
DISCOUNTS = {"SPRING10": 10, "VIP25": 25, "SUMMER20": 20}


def is_valid(code):
    return code in DISCOUNTS


def price(cents, code):
    return round(cents * (100 - DISCOUNTS.get(code, 0)) / 100)


def receipt(code):
    return f"{code} ({DISCOUNTS[code]}% off)" if is_valid(code) else "no discount"


assert price(1000, "SPRING10") == 900
assert price(1000, "SUMMER20") == 800
assert receipt("SUMMER20") == "SUMMER20 (20% off)"
assert price(1000, "BOGUS") == 1000
assert receipt("BOGUS") == "no discount"
print("after: tests pass")
```

```output
before: one of three edits was made
after: tests pass
```

- `DISCOUNTS.get(code, 0)` gives 0 for an unknown code, so the price function needs no separate branch for it.
- In a real program the three functions would be in three modules that import `DISCOUNTS` from one place. The import is then the visible link that the "before" version did not have.
