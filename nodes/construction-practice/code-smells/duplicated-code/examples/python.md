```python run id=extract-shared-weight
import math


# Before: the weight rule is written twice.
def domestic_before(items):
    weight = 0
    for item in items:
        weight += item["kg"] * item["qty"]
    return 5 + math.ceil(weight) * 2


def international_before(items):
    weight = 0
    for item in items:
        weight += item["kg"] * item["qty"]
    return 20 + math.ceil(weight) * 6


# After: the weight rule has one home.
def billable_kg(items):
    return math.ceil(sum(item["kg"] * item["qty"] for item in items))


def domestic(items):
    return 5 + billable_kg(items) * 2


def international(items):
    return 20 + billable_kg(items) * 6


items = [{"kg": 0.4, "qty": 3}, {"kg": 1.1, "qty": 1}]
for name, dom, intl in [
    ("before", domestic_before, international_before),
    ("after", domestic, international),
]:
    assert dom(items) == 11
    assert intl(items) == 38
    assert dom([]) == 5
    print(f"{name}: tests pass")
```

```output
before: tests pass
after: tests pass
```

- `math.ceil` returns an `int`, so the prices are whole numbers and the asserts compare them with `==`.
- The extracted function uses `sum` over a generator. The extract is also the place to make the rule shorter, because there is now one copy to change.
