```python run id=four-rules
MEMBER_DISCOUNT = 0.1


def price_after_discount(amount: float, is_member: bool) -> float:
    qualifies = is_member or amount > 100
    return amount * (1 - MEMBER_DISCOUNT) if qualifies else amount


assert price_after_discount(50, False) == 50
assert price_after_discount(50, True) == 45
assert price_after_discount(200, False) == 180
print(price_after_discount(50, False), price_after_discount(50, True), price_after_discount(200, False))
```

```output
50 45.0 180.0
```
