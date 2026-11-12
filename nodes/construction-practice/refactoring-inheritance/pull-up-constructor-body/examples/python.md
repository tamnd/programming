```python run id=pull-up-constructor
class Party:
    def __init__(self, name: str) -> None:
        self.name = name


class Employee(Party):
    def __init__(self, name: str, id: str, monthly_cost: int) -> None:
        super().__init__(name)
        self.id = id
        self.monthly_cost = monthly_cost


class Department(Party):
    def __init__(self, name: str, staff: list) -> None:
        super().__init__(name)
        self.staff = staff


ana = Employee("Ana", "E1", 1000)
sales = Department("Sales", [ana])
assert next(iter(vars(ana))) == "name"
print(ana.name, sales.name)
```

```output
Ana Sales
```
