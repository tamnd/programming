```python run id=callable-not-checked
import inspect
from collections.abc import Callable


def apply(f: Callable[[int], int], x: int) -> int:
    return f(x)


# Annotations are not checked when the program runs, so a str function runs too.
print(apply(str.upper, "abc"))


# An object with __call__ is callable, so it fits Callable.
class Scale:
    def __init__(self, k):
        self.k = k

    def __call__(self, n):
        return n * self.k


print(apply(Scale(3), 5), callable(Scale(3)), callable(5))
print(inspect.signature(apply))

# Each lambda reads i when it runs, not when it is made.
fs = [lambda: i for i in range(3)]
print([f() for f in fs])
fs = [lambda i=i: i for i in range(3)]
print([f() for f in fs])
```

```output for=callable-not-checked
ABC
15 True False
(f: collections.abc.Callable[[int], int], x: int) -> int
[2, 2, 2]
[0, 1, 2]
```

- Verdict: native. A function is an object, and `Callable[[int], int]` describes it for a type checker. The interpreter does not check the annotation, so `apply(str.upper, "abc")` runs. Any object with a `__call__` method is callable. The lambdas in the first list all read `i` after the loop ends, so they return 2. A default argument stores the value of `i` when each lambda is made.
