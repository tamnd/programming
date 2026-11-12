```python run id=encapsulate-global
# Before: any code can write the global.
tax_rate = 0.2


def price_before(cents):
    return round(cents * (1 + tax_rate))


def export_order_before(cents):
    global tax_rate
    tax_rate = 0  # the export is tax free
    return price_before(cents)  # and nothing puts the rate back


assert price_before(1000) == 1200
assert export_order_before(1000) == 1000
assert price_before(1000) == 1000  # the domestic price has changed
print("before: the export changed the domestic price")


# After: the rate is encapsulated, and the export passes its own rate.
class TaxSettings:
    def __init__(self, rate):
        self._rate = rate

    @property
    def rate(self):
        return self._rate

    @rate.setter
    def rate(self, value):
        if not 0 <= value <= 1:
            raise ValueError(f"tax rate {value}")
        self._rate = value


settings = TaxSettings(0.2)


def price(cents, rate=None):
    if rate is None:
        rate = settings.rate
    return round(cents * (1 + rate))


def export_order(cents):
    return price(cents, rate=0)


assert price(1000) == 1200
assert export_order(1000) == 1000
assert price(1000) == 1200
try:
    settings.rate = 20
except ValueError:
    print("after: tests pass")
else:
    raise AssertionError("rate of 20 accepted")
```

```output
before: the export changed the domestic price
after: tests pass
```

- `export_order_before` needs `global tax_rate`. Without it, the assignment would make a local variable, and the global would not change. The `global` line is the one place where the code says that it writes shared data, and Pylint reports it as `global-statement`.
- The default of `rate` is `None`, and the function reads `settings.rate` at call time. A default of `settings.rate` in the `def` line would be read once, when the function is defined, and later changes to the setting would not be seen.
- The leading underscore in `_rate` is a convention, not a rule that Python enforces. The property is the door that the rest of the code is expected to use.
