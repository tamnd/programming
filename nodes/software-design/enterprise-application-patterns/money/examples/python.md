```python run id=money-allocate
from dataclasses import dataclass

FRACTION_DIGITS = {"EUR": 2, "USD": 2, "JPY": 0}  # from ISO 4217


@dataclass(frozen=True)
class Money:
    units: int  # in the minor unit of the currency, such as cents; never a float
    currency: str

    def __add__(self, other):
        self._same_currency(other)
        return Money(self.units + other.units, self.currency)

    def _same_currency(self, other):
        if self.currency != other.currency:
            raise ValueError(f"cannot add {self.currency} and {other.currency}")

    def allocate(self, ratios):
        """Split by ratios. The sum of the parts is always the whole amount."""
        total = sum(ratios)
        parts = [self.units * r // total for r in ratios]
        for i in range(self.units - sum(parts)):  # the units that are left, one each
            parts[i] += 1
        return [Money(p, self.currency) for p in parts]

    def __str__(self):
        d = FRACTION_DIGITS[self.currency]
        if d == 0:
            return f"{self.units} {self.currency}"
        whole, frac = divmod(self.units, 10**d)
        return f"{whole}.{frac:0{d}d} {self.currency}"


def show(parts):
    return ", ".join(str(p) for p in parts)


a = Money(1000, "EUR")
b = Money(50, "EUR")
print("a:", a)
print("a + b:", a + b)
try:
    a + Money(500, "JPY")
except ValueError as e:
    print("error:", e)
print("fee:", Money(500, "JPY"))
print("5 cents 70/30:", show(Money(5, "EUR").allocate([70, 30])))
print("100 EUR in 3:", show(Money(10000, "EUR").allocate([1, 1, 1])))
print("equal:", "yes" if Money(1050, "EUR") == a + b else "no")
```

```output
a: 10.00 EUR
a + b: 10.50 EUR
error: cannot add EUR and JPY
fee: 500 JPY
5 cents 70/30: 0.04 EUR, 0.01 EUR
100 EUR in 3: 33.34 EUR, 33.33 EUR, 33.33 EUR
equal: yes
```

- Verdict: idiom. `Money` is a frozen data class with an `int` amount in the minor unit and a currency code. Python integers have no fixed size, so the amount cannot overflow.
- The standard library has `decimal.Decimal` for exact decimal amounts, but no money type. The table of fraction digits here has only the three currencies of the example.
