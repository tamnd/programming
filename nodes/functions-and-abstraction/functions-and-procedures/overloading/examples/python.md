```python run id=overloading-python
from functools import singledispatch


@singledispatch
def show(x, y=None):
    raise TypeError(f"no overload for {type(x).__name__}")


@show.register
def _(x: int, y=None):
    if y is None:
        return f"int {x}"
    return f"two ints {x} {y}"


@show.register
def _(x: str, y=None):
    return f"string {x}"


print(show(42))
print(show("hi"))
print(show(3, 4))
```

```output for=overloading-python
int 42
string hi
two ints 3 4
```

- Verdict: stdlib. Python has no overloading. `functools.singledispatch` chooses an implementation from the type of the first argument at run time, and a default value covers the second argument.
