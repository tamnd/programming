```python run id=type-code-to-subclasses
class Employee:
    bonus = 0

    def __init__(self, name: str) -> None:
        self.name = name


class Engineer(Employee):
    bonus = 1000


class Salesman(Employee):
    bonus = 3000


class Manager(Employee):
    bonus = 5000


CLASSES = {"engineer": Engineer, "salesman": Salesman, "manager": Manager}


def create_employee(name: str, type_code: str) -> Employee:
    return CLASSES[type_code](name)


staff = [create_employee("Ana", "salesman"), create_employee("Bo", "manager")]
print([f"{e.name} {type(e).__name__} {e.bonus}" for e in staff])
```

```output
['Ana Salesman 3000', 'Bo Manager 5000']
```
