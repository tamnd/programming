```python run id=explicit-functions
def book_before(customer: str, is_premium: bool) -> dict:
    booking = {"customer": customer, "seat": "standard", "lounge": False}
    if is_premium:
        booking.update(seat="front", lounge=True)
    return booking


def base_booking(customer: str) -> dict:
    return {"customer": customer, "seat": "standard", "lounge": False}


def regular_booking(customer: str) -> dict:
    return base_booking(customer)


def premium_booking(customer: str) -> dict:
    return {**base_booking(customer), "seat": "front", "lounge": True}


# When the flag must stay, make it keyword-only, so that each call names it.
def book(customer: str, *, premium: bool = False) -> dict:
    return premium_booking(customer) if premium else regular_booking(customer)


assert regular_booking("Ana") == book_before("Ana", False)
assert premium_booking("Ana") == book_before("Ana", True)
assert book("Ana", premium=True) == premium_booking("Ana")
try:
    book("Ana", True)
except TypeError as e:
    print(type(e).__name__)
print(premium_booking("Ana"))
```

```output
TypeError
{'customer': 'Ana', 'seat': 'front', 'lounge': True}
```
