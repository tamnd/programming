```python run id=one-band-function
import math


def bottom_band(usage: float) -> float:
    return min(usage, 100)


def middle_band(usage: float) -> float:
    return min(usage, 200) - 100 if usage > 100 else 0


def top_band(usage: float) -> float:
    return usage - 200 if usage > 200 else 0


def within_band(usage: float, bottom: float, top: float) -> float:
    return min(usage, top) - bottom if usage > bottom else 0


for usage in [0, 50, 100, 150, 200, 350]:
    assert within_band(usage, 0, 100) == bottom_band(usage)
    assert within_band(usage, 100, 200) == middle_band(usage)
    assert within_band(usage, 200, math.inf) == top_band(usage)


def charge(usage: float) -> float:
    return (
        within_band(usage, 0, 100) * 0.03
        + within_band(usage, 100, 200) * 0.05
        + within_band(usage, 200, math.inf) * 0.07
    )


assert f"{charge(350):.2f}" == "18.50"
print("parameterize function: tests pass")
```

```output
parameterize function: tests pass
```
