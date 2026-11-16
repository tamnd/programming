```python run id=object-and-any
from typing import Any


def shout_object(x: object) -> str:
    # A type checker allows upper only after the isinstance test.
    if isinstance(x, str):
        return x.upper()
    return str(x)


def shout_any(x: Any) -> str:
    # A type checker accepts this call, even for a value that has no upper method.
    return x.upper()


print(shout_object("hi"), shout_object(3))
print(shout_any("hi"))
try:
    shout_any(3)
except AttributeError as e:
    print("AttributeError:", e)
```

```output for=object-and-any
HI 3
HI
AttributeError: 'int' object has no attribute 'upper'
```

- Verdict: native. `object` is the base class of every class, so it is the top type, and a type checker allows only the operations that every object has until code narrows the type. `Any` also accepts every value, but a type checker does not check its uses, so the error in `shout_any(3)` appears only when the program runs.
