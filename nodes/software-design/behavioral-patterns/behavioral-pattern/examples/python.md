```python run id=kinds
from typing import Callable


class Checkout:
    def total(self, prices: list[int]) -> int:
        return self.discount(sum(prices))

    def discount(self, amount: int) -> int:
        return amount


class SaleCheckout(Checkout):
    def discount(self, amount: int) -> int:
        return amount * 9 // 10


class Till:
    def __init__(self, discount: Callable[[int], int]) -> None:
        self.discount = discount

    def total(self, prices: list[int]) -> int:
        return self.discount(sum(prices))


prices = [40, 60]
print("class pattern:", SaleCheckout().total(prices))
till = Till(lambda amount: amount * 9 // 10)
print("object pattern:", till.total(prices))
till.discount = lambda amount: amount - 20
print("changed at run time:", till.total(prices))
```

```output
class pattern: 90
object pattern: 90
changed at run time: 80
```

- Verdict: native. A function is a value, so the strategy is a lambda and no strategy class is needed. The class pattern uses an ordinary method override.
