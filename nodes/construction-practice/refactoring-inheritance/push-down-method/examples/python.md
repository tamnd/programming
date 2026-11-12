```python run id=push-down
class Employee:
    def __init__(self, name: str) -> None:
        self.name = name


class Engineer(Employee):
    pass


class Salesman(Employee):
    def __init__(self, name: str, quota: int) -> None:
        super().__init__(name)
        self._quota = quota

    @property
    def quota(self) -> int:
        return self._quota


ana, bo = Salesman("Ana", 50000), Engineer("Bo")
assert not hasattr(bo, "quota")
print(ana.name, ana.quota)
```

```output
Ana 50000
```
