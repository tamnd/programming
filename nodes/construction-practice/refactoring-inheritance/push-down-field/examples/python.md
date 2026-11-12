```python run id=push-down-field
class Employee:
    def __init__(self, name: str) -> None:
        self.name = name


class Engineer(Employee):
    pass


class Salesman(Employee):
    def __init__(self, name: str, quota: int) -> None:
        super().__init__(name)
        self.quota = quota


assert list(vars(Engineer("Bo"))) == ["name"]
print(list(vars(Salesman("Ana", 50000))))
```

```output
['name', 'quota']
```
