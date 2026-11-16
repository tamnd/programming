```python run id=cents
from decimal import ROUND_HALF_UP, Decimal


def show(cents):
    sign = "-" if cents < 0 else ""
    cents = abs(cents)
    return f"{sign}{cents // 100}.{cents % 100:02d}"


price = 1999
total = price * 3
print("3 x 19.99:", show(total))
rate = 825
tax = (total * rate + 5000) // 10000
print("tax at 8.25 percent:", show(tax))
print("0.1 + 0.2 as floats:", 0.1 + 0.2)
print("10 + 20 cents:", show(10 + 20))
d = Decimal("59.97") * Decimal("0.0825")
print("Decimal product:", d)
print("quantized:", d.quantize(Decimal("0.01"), rounding=ROUND_HALF_UP))
print("-7 cents // 2:", -7 // 2)
```

```output for=cents
3 x 19.99: 59.97
tax at 8.25 percent: 4.95
0.1 + 0.2 as floats: 0.30000000000000004
10 + 20 cents: 0.30
Decimal product: 4.947525
quantized: 4.95
-7 cents // 2: -4
```

- Verdict: idiom. Python has no fixed-point type, so the block keeps money as an `int` number of cents and the tax rate as an `int` with a scale of 1/10000.
- Adding 5000 before the division by 10000 rounds half up for positive values. `//` is floored, so a negative amount rounds toward minus infinity.
- `decimal.Decimal` with `quantize` gives the same result with a decimal type.
