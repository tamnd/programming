```python run id=subclass-to-delegate
from types import SimpleNamespace

warehouse = SimpleNamespace(days_to_ship=5)
priority_plan = SimpleNamespace(days_to_ship=2)


class PriorityOrderDelegate:
    def __init__(self, host: "Order", plan) -> None:
        self._host = host
        self._plan = plan

    @property
    def days_to_ship(self) -> int:
        return self._plan.days_to_ship


class Order:
    def __init__(self, warehouse) -> None:
        self._warehouse = warehouse
        self._priority_delegate = None

    @property
    def days_to_ship(self) -> int:
        if self._priority_delegate is not None:
            return self._priority_delegate.days_to_ship
        return self._warehouse.days_to_ship

    def make_priority(self, plan) -> None:
        self._priority_delegate = PriorityOrderDelegate(self, plan)


order = Order(warehouse)
days = [order.days_to_ship]
order.make_priority(priority_plan)
days.append(order.days_to_ship)
print(days)
```

```output
[5, 2]
```
