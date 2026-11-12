```python run id=two-jobs
import re
from dataclasses import dataclass

@dataclass(frozen=True)
class Address:
    street: str
    city: str
    postcode: str

    def __post_init__(self):
        if not re.fullmatch(r"\d{5}", self.postcode):
            raise ValueError(f"bad postcode {self.postcode!r}")

    @property
    def label(self):
        return f"{self.street}\n{self.postcode} {self.city.upper()}"

class Order:
    def __init__(self, order_id, address):
        self.order_id = order_id
        self._address = address

    @property
    def address(self):
        return self._address

    def address_label(self):  # kept for the old callers
        return self._address.label

o = Order(1, Address("Main St 5", "Berlin", "10115"))
assert o.address_label() == "Main St 5\n10115 BERLIN"
assert o.address == Address("Main St 5", "Berlin", "10115")  # equality by value
try:
    Address("Main St 5", "Berlin", "1011")
    raise AssertionError("expected a ValueError")
except ValueError as e:
    print(e)
print("extract class: tests pass")
```

```output
bad postcode '1011'
extract class: tests pass
```
