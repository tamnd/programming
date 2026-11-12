```python run id=timedelta-and-newtype
from datetime import timedelta
from typing import NewType

Metres = NewType("Metres", float)
Feet = NewType("Feet", float)


def climb(altitude: Metres, by: Metres) -> Metres:
    return Metres(altitude + by)


timeout = timedelta(seconds=90) + timedelta(milliseconds=500)
print(timeout, "is", timeout.total_seconds(), "seconds")
print("2 min / 30 s =", timedelta(minutes=2) / timedelta(seconds=30))
try:
    timeout + 5
except TypeError as err:
    print("TypeError:", err)

# mypy rejects the Feet argument, but at run time a NewType is a plain float
print("climb:", climb(Metres(1000.0), Feet(500.0)), type(Feet(500.0)).__name__)
```

```output for=timedelta-and-newtype
0:01:30.500000 is 90.5 seconds
2 min / 30 s = 4.0
TypeError: unsupported operand type(s) for +: 'datetime.timedelta' and 'int'
climb: 1500.0 float
```

- Verdict: library. The standard library has a unit type only for time: `timedelta` stores a duration, adds only to other durations and dates, and a duration divided by a duration is a plain number. `NewType` gives a distinct name that mypy checks, but at run time `Feet(500.0)` is a `float`, so the wrong call runs. For physical units in general, the Pint library tracks units and dimensions at run time, and Astropy has a units module.
