```python run id=stop-where-you-know
def first_over_limit_before(orders: list[dict], limit: int) -> str | None:
    found = False
    result = None
    for o in orders:
        if not found:
            if o["total_cents"] > limit:
                result = o["id"]
                found = True
    return result


def first_over_limit(orders: list[dict], limit: int) -> str | None:
    result = None
    for o in orders:
        if o["total_cents"] > limit:
            result = o["id"]
            break
    return result


def describe(orders: list[dict], limit: int) -> str:
    for o in orders:
        if o["total_cents"] > limit:
            message = f"{o['id']} is over the limit"
            break
    else:  # runs only when the loop ends without break
        message = "all orders are within the limit"
    return message


orders = [{"id": "a", "total_cents": 900}, {"id": "b", "total_cents": 1500}]
for limit in (0, 1000, 5000):
    assert first_over_limit(orders, limit) == first_over_limit_before(orders, limit)
assert describe(orders, 1000) == "b is over the limit"
assert describe(orders, 5000) == "all orders are within the limit"
print("replace control flag with break: tests pass")
```

```output
replace control flag with break: tests pass
```
