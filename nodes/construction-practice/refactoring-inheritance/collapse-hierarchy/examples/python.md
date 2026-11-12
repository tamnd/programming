```python run id=collapse
from dataclasses import dataclass


@dataclass
class Employee:
    name: str
    region: str = "none"


ana = Employee("Ana", "north")
assert Employee("Bo").region == "none"
print(ana.name, ana.region)
```

```output
Ana north
```
