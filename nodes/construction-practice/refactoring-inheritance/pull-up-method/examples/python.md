```python run id=pull-up
class Employee:
    def __init__(self, name: str, monthly_cost: int) -> None:
        self.name = name
        self.monthly_cost = monthly_cost

    @property
    def annual_cost(self) -> int:
        return self.monthly_cost * 12


class Salesman(Employee):
    pass


class Engineer(Employee):
    pass


ana, bo = Salesman("Ana", 1000), Engineer("Bo", 1500)
assert "annual_cost" not in vars(Salesman)
print(ana.annual_cost, bo.annual_cost)
```

```output
12000 18000
```
