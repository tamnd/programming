```python run id=rename-in-stages
import math
import warnings

# Before: a cryptic name, and a parameter wider than the need.
def circum(circle):
    return 2 * math.pi * circle["radius"]

# The body moves to a function with the declaration we want.
def circumference(radius):
    return 2 * math.pi * radius

# The old function becomes a wrapper that warns and passes the field.
def circum_old(circle):
    warnings.warn("use circumference(radius)", DeprecationWarning, stacklevel=2)
    return circumference(circle["radius"])

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    for c in ({"radius": 0}, {"radius": 1}, {"radius": 2.5}):
        assert circum_old(c) == circum(c)
        assert circumference(c["radius"]) == circum(c)
assert len(caught) == 3 and caught[0].category is DeprecationWarning

# Adding a parameter: a keyword argument with a default keeps old calls valid.
def price(quantity, unit_price, *, discount=0.0):
    return quantity * unit_price * (1 - discount)

assert price(3, 10) == 30
assert price(3, 10, discount=0.5) == 15
print("change function declaration: tests pass")
```

```output
change function declaration: tests pass
```
