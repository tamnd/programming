```python run id=command-to-function
from types import SimpleNamespace


class ChargeCalculator:
    def __init__(self, customer, usage: float) -> None:
        self._customer = customer
        self._usage = usage

    def execute(self) -> float:
        return self._customer.rate * self._usage


def charge(customer, usage: float) -> float:
    return customer.rate * usage


customer = SimpleNamespace(rate=0.25)
assert charge(customer, 40) == ChargeCalculator(customer, 40).execute()
print(charge(customer, 40))
```

```output
10.0
```
