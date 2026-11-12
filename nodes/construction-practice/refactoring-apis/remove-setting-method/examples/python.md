```python run id=no-setter
from dataclasses import FrozenInstanceError, dataclass


class PersonBefore:
    def __init__(self) -> None:
        self.id = ""


@dataclass(frozen=True)
class Person:
    id: str


before = PersonBefore()
before.id = "1234"
before.id = "9999"
assert before.id == "9999"

ana = Person("1234")
try:
    ana.id = "9999"
except FrozenInstanceError as e:
    print(type(e).__name__)
print(ana.id)
```

```output
FrozenInstanceError
1234
```
