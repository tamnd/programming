```python run id=gone
import importlib


# After: only the live code is left.
def price(cents: int, currency: str) -> int:
    if currency == "EUR":
        return cents
    raise ValueError(f"unsupported currency {currency}")


assert price(1999, "EUR") == 1999
try:
    price(100, "USD")
    raise AssertionError("expected an error")
except ValueError:
    pass


# A caller that a text search for "dumps" may miss: the name is built from strings.
def call_by_name(module: str, func: str, *args):
    return getattr(importlib.import_module(module), func)(*args)


assert call_by_name("js" + "on", "du" + "mps", [1, 2]) == "[1, 2]"
print("remove dead code: tests pass")
```

```output
remove dead code: tests pass
```
