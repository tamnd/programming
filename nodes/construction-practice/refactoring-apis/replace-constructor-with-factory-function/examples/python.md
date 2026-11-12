```python run id=factory-function
class Employee:
    LEGAL_TYPE_CODES = {"E": "Engineer", "M": "Manager", "S": "Salesperson"}

    def __init__(self, name: str, type_code: str) -> None:
        self.name = name
        self._type_code = type_code

    @property
    def type(self) -> str:
        return self.LEGAL_TYPE_CODES[self._type_code]

    @classmethod
    def engineer(cls, name: str) -> "Employee":
        return cls(name, "E")


before = Employee("Ana", "E")
after = Employee.engineer("Ana")
assert vars(after) == vars(before)
print(f"{after.name}: {after.type}")
```

```output
Ana: Engineer
```
