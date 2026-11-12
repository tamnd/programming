```python run id=put-the-body-back
# Before: a helper whose name says the same thing as its body.
def more_than_five_late_deliveries(driver):
    return driver["late_deliveries"] > 5


def rating_before(driver):
    return 2 if more_than_five_late_deliveries(driver) else 1


# A helper that reads its argument twice, called with an argument
# that has a side effect.
reads = 0


def next_reading():
    global reads
    reads += 1
    return 7


def square_before(x):
    return x * x


def score_before():
    return square_before(next_reading())


# After: the body at the call, and the helpers deleted.
def rating(driver):
    return 2 if driver["late_deliveries"] > 5 else 1


def score():
    x = next_reading()  # one evaluation, as in the call
    return x * x


for late in (0, 5, 6, 12):
    assert rating({"late_deliveries": late}) == rating_before({"late_deliveries": late})
reads = 0
assert score_before() == 49 and reads == 1
reads = 0
assert score() == 49 and reads == 1


# Do not inline a method that a subclass overrides.
class Shipping:
    def fee(self, kg):
        return self.base() + 2 * kg

    def base(self):
        return 5


class FreeBase(Shipping):
    def base(self):
        return 0


class ShippingInlined(Shipping):
    def fee(self, kg):
        return 5 + 2 * kg  # base() inlined


assert FreeBase().fee(3) == 6
# The inlined body fixes the base at 5, so a subclass that changes base()
# would no longer be honoured.
class FreeBaseInlined(ShippingInlined):
    def base(self):
        return 0


assert FreeBaseInlined().fee(3) == 11
print("inline: tests pass")
```

```output
inline: tests pass
```

The last part shows why step 2 matters. `base()` is a hook that `FreeBase` overrides. Once `fee` has the body of `Shipping.base` inlined, a subclass that overrides `base` still compiles and runs, but its override is never called.
