```python run id=name-the-parts
from datetime import date


# Before: one expression, three ideas.
def price_before(quantity, item_price):
    return (
        quantity * item_price
        - max(0, quantity - 500) * item_price * 0.05
        + min(quantity * item_price * 0.1, 100)
    )


# After: each idea has a name.
def price(quantity, item_price):
    base_price = quantity * item_price
    quantity_discount = max(0, quantity - 500) * item_price * 0.05
    shipping = min(base_price * 0.1, 100)
    return base_price - quantity_discount + shipping


for q, p in [(1, 10), (500, 2), (501, 2), (2000, 3)]:
    assert price(q, p) == price_before(q, p)


# A condition, named with a boolean variable.
def ships_today_before(order_day, hour, express):
    return (order_day.weekday() < 5 and hour < 14) or (express and hour < 18)


def ships_today(order_day, hour, express):
    is_workday = order_day.weekday() < 5
    before_cutoff = hour < 14
    express_window = express and hour < 18
    return (is_workday and before_cutoff) or express_window


for day in (date(2026, 11, 13), date(2026, 11, 14)):
    for hour in (9, 15, 19):
        for express in (False, True):
            assert ships_today(day, hour, express) == ships_today_before(day, hour, express)
print("price: tests pass")
```

```output
price: tests pass
```

The variables in `ships_today` are safe to extract because each part is a comparison with no side effect. If a part called a function that did work, extracting it would run that work even where `and` or `or` used to skip it.
