```python run id=subclass-to-field
from dataclasses import dataclass


@dataclass(frozen=True)
class Person:
    name: str
    gender_code: str = "X"

    @property
    def is_male(self) -> bool:
        return self.gender_code == "M"


people = [Person("Ana", "F"), Person("Bo", "M")]
assert Person("Cy").gender_code == "X"
print([f"{p.name} {p.gender_code} {p.is_male}" for p in people])
```

```output
['Ana F False', 'Bo M True']
```
