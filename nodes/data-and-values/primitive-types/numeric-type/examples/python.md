```python run id=numbers-tower-and-rounding
import math
import numbers
import sys

# The numbers module is a tower: every Integral is Rational, every Rational is Real.
print(isinstance(3, numbers.Rational), isinstance(3.0, numbers.Rational), isinstance(3.0, numbers.Real), isinstance(1j, numbers.Real))

# Conversion to int truncates toward zero, but // and divmod round toward minus infinity.
print(int(3.9), int(-3.9), -7 // 2, divmod(-7, 2), math.fmod(-7, 2))

# round() rounds a tie to the even neighbor.
print(round(0.5), round(1.5), round(2.5))

# float has infinities and NaN, and NaN is not equal to itself.
inf, nan = float("inf"), float("nan")
print(inf > sys.float_info.max, nan == nan, math.isnan(nan))

# An int grows as needed, so 100 factorial is exact.
f = math.factorial(100)
print(f.bit_length(), str(f)[:12], len(str(f)))

# A float that is too big for an int conversion is still exact as an int.
print(int(1e20), 10**20 == 1e20, 10**23 == 1e23)
```

```output for=numbers-tower-and-rounding
True False True False
3 -3 -4 (-4, 1) -1.0
0 2 2
True False True
525 933262154439 158
100000000000000000000 True False
```

- Verdict: native. The `numbers` module is a tower, so `3` is `Rational` and `3.0` is `Real` but not `Rational`. `int()` truncates toward zero, but `//` and `divmod` round toward minus infinity, and `round()` takes a tie to the even neighbor, so `round(2.5)` is `2`. NaN is not equal to itself. An `int` has no limit, so 100 factorial is exact with 158 digits. `10**20 == 1e20` is true, but `10**23 == 1e23` is false, because `1e23` has no exact binary form.
