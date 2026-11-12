```python run id=extract-superclass
from abc import ABC, abstractmethod


class Party(ABC):
    def __init__(self, name: str) -> None:
        self.name = name

    @property
    @abstractmethod
    def annual_cost(self) -> int: ...


class Employee(Party):
    def __init__(self, name: str, id: str, monthly_cost: int) -> None:
        super().__init__(name)
        self.id = id
        self.monthly_cost = monthly_cost

    @property
    def annual_cost(self) -> int:
        return self.monthly_cost * 12


class Department(Party):
    def __init__(self, name: str, staff: list[Employee]) -> None:
        super().__init__(name)
        self.staff = staff

    @property
    def annual_cost(self) -> int:
        return sum(e.annual_cost for e in self.staff)


try:
    Party("nobody")
except TypeError:
    print("Party is abstract")
staff = [Employee("Ana", "E1", 1000), Employee("Bo", "E2", 1500)]
print([f"{p.name} {p.annual_cost}" for p in [*staff, Department("Sales", staff)]])
```

```output
Party is abstract
['Ana 12000', 'Bo 18000', 'Sales 30000']
```
