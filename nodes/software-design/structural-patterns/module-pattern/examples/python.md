```python run id=counter-module
from types import SimpleNamespace


def _make_counter():
    private_counter = 0

    def change_by(val):
        nonlocal private_counter
        private_counter += val

    def increment():
        change_by(1)

    def decrement():
        change_by(-1)

    def value():
        return private_counter

    return SimpleNamespace(increment=increment, decrement=decrement, value=value)


counter = _make_counter()

print("value:", counter.value())
counter.increment()
counter.increment()
print("value:", counter.value())
counter.decrement()
print("value:", counter.value())
```

```output
value: 0
value: 2
value: 1
```

- Verdict: native. Each `.py` file is a module with its own namespace, so the usual form is a file `counter.py` with a module-level variable and functions. The closure here keeps the example in one file.
- A leading underscore marks a name as private by convention; `from counter import *` does not import names that begin with an underscore.
