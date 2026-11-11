```python run id=representation
import math
from decimal import Decimal
from fractions import Fraction

x = 0.1 + 0.2
print("0.1 + 0.2        =", x)
print("== 0.3           :", x == 0.3)
print("isclose(x, 0.3)  :", math.isclose(x, 0.3))
print("exact value of 0.1:", Decimal(0.1))
print("as a fraction     :", Fraction(0.1))
print("hex form of 0.1   :", (0.1).hex())
print("ulp(1.0)          :", math.ulp(1.0))
print("next after 1.0    :", math.nextafter(1.0, 2.0))
```

```output
0.1 + 0.2        = 0.30000000000000004
== 0.3           : False
isclose(x, 0.3)  : True
exact value of 0.1: 0.1000000000000000055511151231257827021181583404541015625
as a fraction     : 3602879701896397/36028797018963968
hex form of 0.1   : 0x1.999999999999ap-4
ulp(1.0)          : 2.220446049250313e-16
next after 1.0    : 1.0000000000000002
```

- Verdict: native. A Python `float` is an IEEE 754 binary64 value on almost all platforms.
- `repr` prints the shortest string that reads back as the same value, so `0.1` prints as `0.1` though the stored value is not 0.1.
- `Decimal(0.1)` and `Fraction(0.1)` show the exact value that the float holds. `float.hex` shows its bits as a hexadecimal significand and a binary exponent.
- `math.isclose` compares with a relative tolerance of `1e-09` by default.

Rounding errors add up in a long sum. A plain loop rounds after each addition. Since Python 3.12, the built-in `sum` uses Neumaier summation for floats, which keeps most of the lost low-order parts. `math.fsum` tracks all of them and rounds once at the end.

```python run id=summation
import math

def loop_sum(values):
    total = 0.0
    for v in values:
        total += v
    return total

for values in ([0.1] * 10, [1e16, 1.0, -1e16]):
    print(values if len(values) < 5 else "ten times 0.1")
    print("  loop     :", loop_sum(values))
    print("  sum      :", sum(values))
    print("  math.fsum:", math.fsum(values))
```

```output
ten times 0.1
  loop     : 0.9999999999999999
  sum      : 1.0
  math.fsum: 1.0
[1e+16, 1.0, -1e+16]
  loop     : 0.0
  sum      : 1.0
  math.fsum: 1.0
```

For money, use a decimal type. `Decimal("0.10")` holds exactly ten cents, and the context sets the rounding rule.

```python run id=money
from decimal import Decimal, ROUND_HALF_EVEN

price = Decimal("0.10")
total = sum([price] * 3, Decimal("0"))
print("total:", total, total == Decimal("0.30"))
print("Decimal 2.675 to 0.01:", Decimal("2.675").quantize(Decimal("0.01"), rounding=ROUND_HALF_EVEN))
print("float round(2.675, 2):", round(2.675, 2))
```

```output
total: 0.30 True
Decimal 2.675 to 0.01: 2.68
float round(2.675, 2): 2.67
```

- `round(2.675, 2)` gives `2.67`, not `2.68`. The float nearest to 2.675 is a little below it, so the correct rounding of the stored value is down.
