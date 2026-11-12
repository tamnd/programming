```python run id=delete-what-does-not-run
import sys

ACCEPTED = {"EUR"}


def check_currency(c):
    if c not in ACCEPTED:
        raise ValueError(f"currency {c} is not accepted")
    return c


# Before: unreachable branches, an unused helper and an unread value.
def legacy_round_usd(cents):
    return round(cents / 5) * 5


def price_before(cents, currency):
    check_currency(currency)
    with_tax = round(cents * 1.2)
    display = f"{with_tax / 100:.2f}"  # never read
    if currency == "USD":
        return (with_tax + 30, currency)
    if currency == "GBP":
        return (with_tax + 20, currency)
    return (with_tax, currency)


# After: only the code that runs.
def price(cents, currency):
    check_currency(currency)
    return (round(cents * 1.2), currency)


# A small line tracer, in the spirit of coverage.py: which lines of
# price_before does a run of the tests execute?
code = price_before.__code__
ran = set()


def tracer(frame, event, arg):
    if frame.f_code is code and event == "line":
        ran.add(frame.f_lineno - code.co_firstlineno)
    return tracer


sys.settrace(tracer)
for cents in (0, 1, 999, 1250):
    assert price(cents, "EUR") == price_before(cents, "EUR")
for c in ("USD", "GBP"):
    for f in (price, price_before):
        try:
            f(100, c)
            raise AssertionError("should refuse " + c)
        except ValueError:
            pass
sys.settrace(None)

# Lines 5 and 7, the USD and GBP returns, never ran.
assert 5 not in ran and 7 not in ran and 8 in ran, sorted(ran)
print("price: tests pass")
```

```output
price: tests pass
```

The tracer is a small version of what Coverage.py does: it records which lines run. The two `return` lines for dollars and pounds never run, because `check_currency` refuses those currencies first. Coverage.py gives the same answer for a whole program, with `coverage run` and `coverage report -m`.
