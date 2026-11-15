```python run id=record
from dataclasses import FrozenInstanceError, dataclass, replace


@dataclass(frozen=True)
class Person:
    name: str
    age: int


def yn(b: bool) -> str:
    return "yes" if b else "no"


ada = Person(name="Ada", age=36)
print("name:", ada.name, "age:", ada.age)

older = replace(ada, age=ada.age + 1)
print("copy:", older)
print("original:", ada)
print("same fields equal:", yn(ada == Person("Ada", 36)))

try:
    ada.age = 40
except FrozenInstanceError:
    print("assignment refused: yes")
```

```output for=record
name: Ada age: 36
copy: Person(name='Ada', age=37)
original: Person(name='Ada', age=36)
same fields equal: yes
assignment refused: yes
```

- Verdict: native. A dataclass is a record with generated `__init__`, `__repr__` and `__eq__` methods.
- `replace` makes a new object and leaves the original unchanged.
- `frozen=True` makes an assignment to a field raise `FrozenInstanceError`.
