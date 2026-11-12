```python run id=order-data
from dataclasses import dataclass


@dataclass(frozen=True)
class OrderData:
    order_id: int
    customer: str


class Order:
    def __init__(self, order_id: int, customer: str, items: list[str]) -> None:
        self._data = OrderData(order_id, customer)  # fixed after construction
        self.items = list(items)  # changes

    @property
    def order_id(self) -> int:
        return self._data.order_id

    @property
    def customer(self) -> str:
        return self._data.customer

    def add(self, item: str) -> None:
        self.items.append(item)

    def describe(self) -> str:
        return f"order {self.order_id} for {self.customer}: {len(self.items)} items"


order = Order(42, "ann", ["pen", "ink"])
print(order.describe())
order.add("paper")
print(order.describe())
```

```output
order 42 for ann: 2 items
order 42 for ann: 3 items
```

- Verdict: idiom. A frozen `dataclass` raises `FrozenInstanceError` on assignment, and the properties show the fields for reading only.
- `object.__setattr__` can still write a frozen field, so the protection stops mistakes, not a determined caller.
