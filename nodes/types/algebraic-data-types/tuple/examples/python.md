```python run id=pair
from typing import NamedTuple


class MinMax(NamedTuple):
    lo: int
    hi: int


def min_max(values: list[int]) -> tuple[int, int]:
    return min(values), max(values)


def yn(b: bool) -> str:
    return "yes" if b else "no"


pair = min_max([7, 2, 9, 4])
print("pair:", pair)
lo, hi = pair
print("lo:", lo, "hi:", hi)
print("second part:", pair[1])
print("equal pairs:", yn(pair == (2, 9)))

named = MinMax(lo, hi)
print("named:", named, "hi =", named.hi, "equal to plain:", yn(named == pair))
```

```output for=pair
pair: (2, 9)
lo: 2 hi: 9
second part: 9
equal pairs: yes
named: MinMax(lo=2, hi=9) hi = 9 equal to plain: yes
```

- Verdict: native. A `tuple` is immutable, and `return a, b` builds one without parentheses.
- `lo, hi = pair` unpacks the parts. The number of names must match the number of parts.
- A `NamedTuple` adds names, but it is still a tuple, so it equals a plain tuple with the same parts.
