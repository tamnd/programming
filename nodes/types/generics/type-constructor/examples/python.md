```python run id=origin-args
from collections.abc import Callable
from typing import get_args, get_origin

# Each type is a constructor applied to arguments.
for t in (list[int], dict[str, list[int]], tuple[int, str], Callable[[int], str]):
    print(t, "=", get_origin(t), get_args(t))

# int is a nullary constructor: it takes no arguments.
print(int, get_origin(int), get_args(int))
```

```output for=origin-args
list[int] = <class 'list'> (<class 'int'>,)
dict[str, list[int]] = <class 'dict'> (<class 'str'>, list[int])
tuple[int, str] = <class 'tuple'> (<class 'int'>, <class 'str'>)
collections.abc.Callable[[int], str] = <class 'collections.abc.Callable'> ([<class 'int'>], <class 'str'>)
<class 'int'> None ()
```

- Verdict: native. `get_origin` returns the type constructor and `get_args` returns the type arguments. `list` takes one type, `dict` takes two, and `tuple` takes any number. `Callable` takes the list of parameter types and the result type. `int` is not built from other types, so it has no origin and no arguments.
