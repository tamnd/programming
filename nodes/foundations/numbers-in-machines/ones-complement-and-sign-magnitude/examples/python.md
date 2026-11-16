```python run id=forms
import math


def to_sign_magnitude(n):
    return 0x80 | -n if n < 0 else n


def to_ones(n):
    return ~(-n) & 0xFF if n < 0 else n


def from_sign_magnitude(b):
    return -(b & 0x7F) if b & 0x80 else b


def from_ones(b):
    return -(~b & 0xFF) if b & 0x80 else b


print(f"-43 in sign-magnitude: {to_sign_magnitude(-43):08b}")
print(f"-43 in ones' complement: {to_ones(-43):08b}")
print(f"10101011 in sign-magnitude: {from_sign_magnitude(0b10101011)}")
print(f"11010100 in ones' complement: {from_ones(0b11010100)}")
print(f"10000000 in sign-magnitude: {from_sign_magnitude(0b10000000)}")
print(f"11111111 in ones' complement: {from_ones(0b11111111)}")

z = -0.0
print("-0.0 == 0.0:", "yes" if z == 0.0 else "no")
print("-0.0 prints as:", z)
print("math.copysign(1.0, -0.0):", math.copysign(1.0, z))
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
-0.0 == 0.0: yes
-0.0 prints as: -0.0
math.copysign(1.0, -0.0): -1.0
```

- Verdict: idiom. Python's `int` acts as a two's complement value in bitwise operations, so a program makes the other two forms by hand with `|`, `~` and `&`.
- The two negative zeros become the integer 0, because Python `int` has one zero.
- A float `-0.0` is equal to `0.0`, but it keeps its sign bit. `math.copysign` reads that bit.
