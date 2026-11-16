```python run id=fraction
from fractions import Fraction

print("Fraction(16, -10):", Fraction(16, -10))
s = Fraction(1, 10) + Fraction(2, 10)
print("1/10 + 2/10:", s, "equal to 3/10:", "yes" if s == Fraction(3, 10) else "no")
print("Fraction(1.1):", Fraction(1.1))
print("Fraction('1.1'):", Fraction("1.1"))
print("Fraction(1.1).limit_denominator():", Fraction(1.1).limit_denominator())
pi = Fraction("3.1415926535897932")
print("pi.limit_denominator(1000):", pi.limit_denominator(1000))
t = Fraction(1, 3) ** 40
print("digits of (1/3)**40 denominator:", len(str(t.denominator)))
print("float(Fraction(1, 3)):", float(Fraction(1, 3)))
try:
    Fraction(1, 0)
except ZeroDivisionError as e:
    print("Fraction(1, 0):", type(e).__name__)
```

```output for=fraction
Fraction(16, -10): -8/5
1/10 + 2/10: 3/10 equal to 3/10: yes
Fraction(1.1): 2476979795053773/2251799813685248
Fraction('1.1'): 11/10
Fraction(1.1).limit_denominator(): 11/10
pi.limit_denominator(1000): 355/113
digits of (1/3)**40 denominator: 20
float(Fraction(1, 3)): 0.3333333333333333
Fraction(1, 0): ZeroDivisionError
```

- Verdict: stdlib. `fractions.Fraction` keeps the value in lowest terms with a positive denominator, and its parts are `int` values, so they do not overflow.
- `Fraction(1.1)` is the exact value of the float, and `Fraction("1.1")` is 11/10.
- `limit_denominator` finds a near fraction with a small denominator.
