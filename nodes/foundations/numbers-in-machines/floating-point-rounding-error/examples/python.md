```python run id=money
from decimal import Decimal

prices = [0.1] * 10
print("float sum:", sum(prices), "loop:", end=" ")
total = 0.0
for p in prices:
    total += p
print(total)

exact = sum([Decimal("0.1")] * 10)
print("Decimal sum:", exact)
print("Decimal(0.1):", Decimal(0.1))
print("cents:", sum([10] * 10), "->", f"{sum([10] * 10) / 100:.2f}")
print("round(2.675, 2):", round(2.675, 2), "stored:", Decimal(2.675))
```

```output for=money
float sum: 1.0 loop: 0.9999999999999999
Decimal sum: 1.0
Decimal(0.1): 0.1000000000000000055511151231257827021181583404541015625
cents: 100 -> 1.00
round(2.675, 2): 2.67 stored: 2.67499999999999982236431605997495353221893310546875
```

- Verdict: native. A Python `float` is a binary64 value, so the error is part of every float operation.
- The built-in `sum` uses extended precision for the intermediate steps, so it gives 1.0 where the plain loop gives 0.9999999999999999.
- `Decimal("0.1")` from a string is exact. `Decimal(0.1)` from a float shows the error that the float already has.
- `round(2.675, 2)` gives 2.67 because the stored value is below 2.675.
