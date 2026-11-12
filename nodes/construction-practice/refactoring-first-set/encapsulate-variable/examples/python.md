```python run id=one-way-in
import copy

# A module value behind two functions. The leading underscore marks it private.
_tax_settings = {"rate": 0.2, "region": "EU"}

def tax_settings():
    return copy.deepcopy(_tax_settings)  # a reader cannot change ours

def set_tax_settings(value):
    global _tax_settings
    if not 0 <= value["rate"] < 1:
        raise ValueError(f"bad tax rate {value['rate']}")
    _tax_settings = copy.deepcopy(value)

def price_with_tax(net):
    return net * (1 + tax_settings()["rate"])

mine = tax_settings()
mine["rate"] = 0.9
assert tax_settings()["rate"] == 0.2
try:
    set_tax_settings({"rate": 1.5, "region": "EU"})
    raise AssertionError("the check did not run")
except ValueError:
    pass
set_tax_settings({"rate": 0.1, "region": "EU"})
assert round(price_with_tax(100)) == 110

# In a class, a property keeps the attribute syntax for the callers.
class Account:
    def __init__(self, limit):
        self.limit = limit  # goes through the setter below

    @property
    def limit(self):
        return self._limit

    @limit.setter
    def limit(self, value):
        if value < 0:
            raise ValueError("limit must not be negative")
        self._limit = value

a = Account(100)
a.limit = 50  # the caller's code is the same as before the refactoring
assert a.limit == 50
try:
    a.limit = -1
    raise AssertionError("the check did not run")
except ValueError:
    pass
print("encapsulate variable: tests pass")
```

```output
encapsulate variable: tests pass
```
