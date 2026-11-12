```python run id=pull-up-field
class Employee:
    def __init__(self, name: str) -> None:
        self.name = name


class Salesman(Employee):
    pass


class Engineer(Employee):
    pass


print([e.name for e in (Salesman("Ana"), Engineer("Bo"))])
```

```output
['Ana', 'Bo']
```
