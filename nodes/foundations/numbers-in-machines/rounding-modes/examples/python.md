```python run id=ties
from decimal import (
    ROUND_05UP,
    ROUND_CEILING,
    ROUND_DOWN,
    ROUND_FLOOR,
    ROUND_HALF_DOWN,
    ROUND_HALF_EVEN,
    ROUND_HALF_UP,
    ROUND_UP,
    Decimal,
    localcontext,
)

modes = [ROUND_HALF_EVEN, ROUND_HALF_UP, ROUND_HALF_DOWN, ROUND_UP, ROUND_DOWN, ROUND_CEILING, ROUND_FLOOR, ROUND_05UP]
for mode in modes:
    row = [str(Decimal(s).quantize(Decimal(1), rounding=mode)) for s in ["2.5", "-2.5", "2.6", "-2.4"]]
    print(f"{mode:16} {' '.join(f'{v:>3}' for v in row)}")


def drift(mode, steps):
    with localcontext() as ctx:
        ctx.prec = 3
        ctx.rounding = mode
        x = Decimal("1.00")
        y = Decimal("-0.555")
        for _ in range(steps):
            x = (x - y) + y
        return x


print("Goldberg, 10 steps, half up:  ", drift(ROUND_HALF_UP, 10))
print("Goldberg, 10 steps, half even:", drift(ROUND_HALF_EVEN, 10))
print("round(2.675, 2):", round(2.675, 2))
print("Decimal('2.675'), half up:", Decimal("2.675").quantize(Decimal("0.01"), rounding=ROUND_HALF_UP))
```

```output for=ties
ROUND_HALF_EVEN    2  -2   3  -2
ROUND_HALF_UP      3  -3   3  -2
ROUND_HALF_DOWN    2  -2   3  -2
ROUND_UP           3  -3   3  -3
ROUND_DOWN         2  -2   2  -2
ROUND_CEILING      3  -2   3  -2
ROUND_FLOOR        2  -3   2  -3
ROUND_05UP         2  -2   2  -2
Goldberg, 10 steps, half up:   1.10
Goldberg, 10 steps, half even: 1.00
round(2.675, 2): 2.67
Decimal('2.675'), half up: 2.68
```

- Verdict: native. The built-in `round` uses half to even, and the `decimal` module has eight rounding modes.
- In `decimal`, `ROUND_HALF_UP` means ties away from zero, so -2.5 becomes -3. `ROUND_UP` means away from zero for every value, not only ties.
- The drift test repeats Goldberg's example with three digits. Under half up the value climbs by 0.01 per step. Under half to even it stays at 1.00.
- `round(2.675, 2)` gives 2.67 because the float is a little below 2.675. A `Decimal` built from the string rounds to 2.68.
