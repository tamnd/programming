```python run id=orders
from dataclasses import dataclass, replace
from typing import Protocol


@dataclass
class Order:
    id: int
    customer: str
    total: int
    status: str = "open"

    def ship(self) -> None:
        if self.status != "open":
            raise ValueError(f"order {self.id} is {self.status}")
        self.status = "shipped"


class OrderRepository(Protocol):
    """The domain's view of storage: a collection of orders."""

    def add(self, order: Order) -> None: ...
    def get(self, order_id: int) -> Order | None: ...
    def by_customer(self, customer: str) -> list[Order]: ...
    def update(self, order: Order) -> None: ...


class InMemoryOrderRepository:
    """Stores copies, so a caller must call update, as with a database."""

    def __init__(self) -> None:
        self._rows: dict[int, Order] = {}

    def add(self, order: Order) -> None:
        if order.id in self._rows:
            raise KeyError(f"order {order.id} exists")
        self._rows[order.id] = replace(order)

    def get(self, order_id: int) -> Order | None:
        row = self._rows.get(order_id)
        return replace(row) if row else None

    def by_customer(self, customer: str) -> list[Order]:
        return [replace(o) for o in sorted(self._rows.values(), key=lambda o: o.id) if o.customer == customer]

    def update(self, order: Order) -> None:
        if order.id not in self._rows:
            raise KeyError(f"order {order.id} does not exist")
        self._rows[order.id] = replace(order)


def ship_order(orders: OrderRepository, order_id: int) -> str:
    """Application code. It knows the repository, not the storage."""
    order = orders.get(order_id)
    if order is None:
        return f"order {order_id}: not found"
    order.ship()
    orders.update(order)
    return f"order {order_id} is {order.status}"


repo = InMemoryOrderRepository()
for order in [Order(1, "ana", 30), Order(2, "bo", 12), Order(3, "ana", 5)]:
    repo.add(order)
print("ana:", ", ".join(f"order {o.id} ({o.total})" for o in repo.by_customer("ana")))
print(ship_order(repo, 1))
print(ship_order(repo, 9))
stored = repo.get(1)
print(f"stored: order {stored.id} is {stored.status}")
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```


- Verdict: idiom. A `Protocol` states the collection-like interface, and any class with those methods is a repository. The node page runs the same `ship_order` against a SQLite repository.
- The in-memory class returns copies. A caller that changes an order and forgets `update` sees the change lost, as it would be with a database. The node page shows what goes wrong when a fake returns live objects.
- `ship` is domain logic and stays on `Order`. The repository has no business rules: it stores and finds.
