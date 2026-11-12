```python run id=sales-store
from typing import Protocol


class SalesStore(Protocol):
    def sales_on(self, day: str) -> list[int]: ...


def daily_report(store: SalesStore, day: str) -> str:
    sales = store.sales_on(day)
    return f"{day}: {len(sales)} sales, total {sum(sales)}"


class ListStore:  # no base class: it matches the protocol by its methods
    def __init__(self, rows: list[tuple[str, int]]) -> None:
        self.rows = rows

    def sales_on(self, day: str) -> list[int]:
        return [amount for d, amount in self.rows if d == day]


class FakeStore:
    def sales_on(self, day: str) -> list[int]:
        return [5, 7]


store = ListStore([("mon", 5), ("mon", 7), ("tue", 1)])
assert daily_report(store, "mon") == daily_report(FakeStore(), "mon")
print(daily_report(store, "mon"))
```

```output
mon: 2 sales, total 12
```
