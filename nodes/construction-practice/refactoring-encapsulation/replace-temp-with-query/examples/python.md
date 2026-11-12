```python run id=ask-for-it
from functools import cached_property

class Rental:
    def __init__(self, days, daily_rate):
        self.days = days
        self.daily_rate = daily_rate

    @property
    def subtotal(self):  # a query, computed when it is asked
        return self.days * self.daily_rate

    @property
    def fee(self):
        return 0 if self.subtotal > 500 else 25

    def total(self):
        return self.subtotal + self.fee

r = Rental(2, 100)
assert (r.subtotal, r.fee, r.total()) == (200, 25, 225)
r.days = 6  # a query follows the change; a temp taken before would not
assert r.total() == 600

class Quote:
    """An object that does not change, so a costly value can be cached."""
    calls = 0

    def __init__(self, items):
        self._items = tuple(items)

    @cached_property
    def subtotal(self):
        Quote.calls += 1
        return sum(self._items)

q = Quote([120, 80])
assert q.subtotal == q.subtotal == 200
assert Quote.calls == 1  # computed once
print("replace temp with query: tests pass")
```

```output
replace temp with query: tests pass
```
