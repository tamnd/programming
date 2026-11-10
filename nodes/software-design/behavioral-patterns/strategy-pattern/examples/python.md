```python run id=checkout
from collections.abc import Callable
from dataclasses import dataclass


@dataclass(frozen=True)
class Item:
    name: str
    price: int  # in cents
    quantity: int


Pricing = Callable[[list[Item]], int]


def regular(items: list[Item]) -> int:
    return sum(item.price * item.quantity for item in items)


def ten_percent_off(items: list[Item]) -> int:
    return regular(items) * 90 // 100


def bulk(items: list[Item]) -> int:
    # 50 cents off each unit of a line with 5 or more units.
    return sum(
        (item.price - 50 if item.quantity >= 5 else item.price) * item.quantity
        for item in items
    )


class Checkout:
    def __init__(self, pricing: Pricing) -> None:
        self.pricing = pricing

    def total(self, items: list[Item]) -> int:
        return self.pricing(items)


cart = [Item("book", 1500, 2), Item("pen", 200, 5)]
checkout = Checkout(regular)
for name, pricing in [("regular", regular), ("ten percent off", ten_percent_off), ("bulk", bulk)]:
    checkout.pricing = pricing
    print(f"{name}: {checkout.total(cart)} cents")
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. Python has no strategy construct. A function is a value, so each strategy is a plain function and the interface is the type alias `Pricing`.
- The checkout changes its strategy at run time by assignment to `checkout.pricing`.
- `typing.Protocol` with a `__call__` method or with named methods is the class form, for a strategy with more than one operation.
- The standard library uses the same form: `sorted(items, key=...)` takes the key function as a strategy.
