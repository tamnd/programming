```python run id=floored
import math

print("-7 // 3:", -7 // 3)
print("-7 % 3:", -7 % 3)
print("divmod(-7, 3):", divmod(-7, 3))
print("7 % -3:", 7 % -3)
print("int(-7 / 3):", int(-7 / 3))
print("math.fmod(-7, 3):", math.fmod(-7, 3))
print("-7 is odd:", "yes" if -7 % 2 == 1 else "no")
try:
    print(1 // 0)
except ZeroDivisionError as e:
    print("1 // 0:", type(e).__name__)
```

```output for=floored
-7 // 3: -3
-7 % 3: 2
divmod(-7, 3): (-3, 2)
7 % -3: -2
int(-7 / 3): -2
math.fmod(-7, 3): -1.0
-7 is odd: yes
1 // 0: ZeroDivisionError
```

- Verdict: native. `//` and `%` use floored division, so the remainder has the sign of the divisor, and `divmod` gives both results at once.
- `int(-7 / 3)` truncates a float quotient, and `math.fmod` gives the truncated remainder as a float. For large integers, `int(a / b)` can be wrong, because `/` gives a float.
- The test `n % 2 == 1` works for negative `n` in Python, but not in the truncated languages.
