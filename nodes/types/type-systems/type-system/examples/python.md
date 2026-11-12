```python run id=dynamic-checks-and-duck-typing
from typing import Protocol, get_type_hints, runtime_checkable


# Python checks types when an operation runs, not before.
def describe(x, verbose=False):
    if verbose:
        return x + 1  # a type error here only matters if this line runs
    return f"value {x!r}"


print(describe("a"))
try:
    describe("a", verbose=True)
except TypeError as e:
    print("TypeError:", e)


# Duck typing: any object with the right method works.
class Ring:
    def __len__(self):
        return 3


print(len([1, 2]), len("abc"), len(Ring()))


# A Protocol describes a structure; runtime_checkable lets isinstance test it.
@runtime_checkable
class Sized(Protocol):
    def __len__(self) -> int: ...


print(isinstance(Ring(), Sized), isinstance(42, Sized))


# Annotations are data for a separate checker, and Python does not enforce them.
def area(w: float, h: float) -> float:
    return w * h


print(area("ab", 2), get_type_hints(area))
```

```output for=dynamic-checks-and-duck-typing
value 'a'
TypeError: can only concatenate str (not "int") to str
2 3 3
True False
abab {'w': <class 'float'>, 'h': <class 'float'>, 'return': <class 'float'>}
```

- Verdict: native. Python checks types when an operation runs, so `describe("a")` works and the type error in the other branch appears only when that branch runs. `len` works for any object with `__len__`, which is duck typing. A `runtime_checkable` `Protocol` tests the structure of an object with `isinstance`. Python does not enforce annotations, so `area("ab", 2)` returns `"abab"`, and `get_type_hints` reads them as data.
