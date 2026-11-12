```python run id=annotations-and-boundary-checks
import inspect
from typing import Any, get_type_hints


def area(width: int, height: int) -> int:
    return width * height


print(get_type_hints(area))
print(area("ab", 3))  # Python does not check annotations when it runs


def checked(fn):  # check the annotations at the boundary, as a cast would
    hints, sig = get_type_hints(fn), inspect.signature(fn)

    def wrapper(*args):
        for name, value in sig.bind(*args).arguments.items():
            want = hints.get(name, Any)
            if want is not Any and not isinstance(value, want):
                raise TypeError(f"{fn.__name__}: {name} must be {want.__name__}, "
                                f"got {type(value).__name__}")
        return fn(*args)
    return wrapper


safe_area = checked(area)
print(safe_area(4, 5))
try:
    safe_area("ab", 3)
except TypeError as err:
    print(err)
```

```output for=annotations-and-boundary-checks
{'width': <class 'int'>, 'height': <class 'int'>, 'return': <class 'int'>}
ababab
20
area: width must be int, got str
```

- Verdict: native. Python has annotations in its syntax, `typing.Any` as the unknown type, and separate checkers such as mypy and Pyright that check annotated code and trust unannotated code. The interpreter does not check annotations, so `area("ab", 3)` runs and repeats the string. The decorator reads the annotations with `get_type_hints` and checks the arguments at the call, which is the cast that a sound gradual system would insert. Libraries such as typeguard and beartype do this for real code.
