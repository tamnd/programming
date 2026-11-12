```python run id=query-not-parameter
class Employee:
    def __init__(self, name: str, grade: int) -> None:
        self.name = name
        self._grade = grade

    @property
    def grade(self) -> int:
        return self._grade


def available_vacation_before(employee: Employee, grade: int) -> int:
    return 20 + 2 * grade


def available_vacation(employee: Employee) -> int:
    return 20 + 2 * employee.grade


ana = Employee("Ana", 3)
assert available_vacation(ana) == available_vacation_before(ana, ana.grade)
assert available_vacation_before(ana, 5) != available_vacation(ana)
print(available_vacation(ana))
```

```output
26
```
