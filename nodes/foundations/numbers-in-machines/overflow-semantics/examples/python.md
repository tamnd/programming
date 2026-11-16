```python run id=int32
import ctypes

INT32_MIN, INT32_MAX = -2**31, 2**31 - 1


def wrap32(v):
    return (v - INT32_MIN) % 2**32 + INT32_MIN


def saturate32(v):
    return max(INT32_MIN, min(INT32_MAX, v))


x = INT32_MAX
print("x + 1:", x + 1)
print("wrapped to 32 bits:", wrap32(x + 1))
print("ctypes.c_int32(x + 1):", ctypes.c_int32(x + 1).value)
print("saturated to 32 bits:", saturate32(x + 1))
try:
    (x + 1).to_bytes(4, "big", signed=True)
except OverflowError as e:
    print("to_bytes in 4 bytes:", type(e).__name__)
```

```output for=int32
x + 1: 2147483648
wrapped to 32 bits: -2147483648
ctypes.c_int32(x + 1): -2147483648
saturated to 32 bits: 2147483647
to_bytes in 4 bytes: OverflowError
```

- Verdict: idiom. Python's `int` never overflows, so a program that must act like a 32-bit type wraps, saturates or traps by hand.
- `ctypes.c_int32` keeps the low 32 bits of a value and does not check for overflow.
- `int.to_bytes` traps: it raises `OverflowError` when the value does not fit in the bytes.
