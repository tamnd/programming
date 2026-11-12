```python run id=extract-the-promotion
from dataclasses import dataclass


def sum_cents(items):
    return sum(cents * qty for cents, qty in items)


# Before: the promotion fields are None for most orders.
class OrderBefore:
    def __init__(self, items):
        self.items = items
        self.promo_code = None
        self.promo_percent = 0
        self.promo_until = None

    def apply_promo(self, code, percent, until):
        self.promo_code, self.promo_percent, self.promo_until = code, percent, until

    def total(self, day):
        s = sum_cents(self.items)
        if self.promo_code is not None and day <= self.promo_until:
            return s - round(s * self.promo_percent / 100)
        return s

    def label(self):
        return "no promotion" if self.promo_code is None else f"promotion {self.promo_code}"


# After: the fields have a class, and the empty case has an object.
@dataclass(frozen=True)
class Promotion:
    code: str
    percent: int
    until: str

    def apply(self, s, day):
        return s - round(s * self.percent / 100) if day <= self.until else s

    @property
    def label(self):
        return f"promotion {self.code}"


class NoPromotion:
    label = "no promotion"

    def apply(self, s, day):
        return s


NO_PROMOTION = NoPromotion()


class Order:
    def __init__(self, items, promotion=NO_PROMOTION):
        self.items = items
        self.promotion = promotion

    def total(self, day):
        return self.promotion.apply(sum_cents(self.items), day)

    def label(self):
        return self.promotion.label


items = [(1250, 2), (499, 1)]
plain_before = OrderBefore(items)
promo_before = OrderBefore(items)
promo_before.apply_promo("SPRING", 10, "2026-04-30")
plain = Order(items)
promo = Order(items, Promotion("SPRING", 10, "2026-04-30"))

for day in ["2026-04-01", "2026-05-01"]:
    assert plain.total(day) == plain_before.total(day)
    assert promo.total(day) == promo_before.total(day)
assert promo.total("2026-04-01") == 2699
assert promo.label() == promo_before.label()
assert plain.label() == "no promotion"
print("promotion: tests pass")
```

```output
promotion: tests pass
```

- `Promotion` is a frozen dataclass, so its three fields are always set together and cannot change one at a time.
- `NO_PROMOTION` is one shared object. It has the same two members as `Promotion`, so `Order` calls them with no check for `None`.
- Python's `round` rounds a half to the even number, while JavaScript's `Math.round` rounds a half up. The totals in this example have no halves, so the two agree.
