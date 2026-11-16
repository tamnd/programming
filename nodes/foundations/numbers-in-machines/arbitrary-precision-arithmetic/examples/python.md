```python run id=unlimited
import math
import sys

print("2**64:", 2**64)
print("2**100:", 2**100)
print("30!:", math.factorial(30))
print("(2**100).bit_length():", (2**100).bit_length())
print("10**30 // 7:", 10**30 // 7)
print("str limit:", sys.get_int_max_str_digits())
big = 10**5000
try:
    print(len(str(big)))
except ValueError as e:
    print("str(10**5000):", type(e).__name__)
print("hex digits of 10**5000:", len(hex(big)) - 2)
```

```output for=unlimited
2**64: 18446744073709551616
2**100: 1267650600228229401496703205376
30!: 265252859812191058636308480000000
(2**100).bit_length(): 101
10**30 // 7: 142857142857142857142857142857
str limit: 4300
str(10**5000): ValueError
hex digits of 10**5000: 4153
```

- Verdict: native. Every `int` is a bignum, so `2**100` and `30!` need no special type.
- CPython limits the conversion of an `int` to a decimal string to 4300 digits by default, and raises `ValueError` above it. The conversion to hexadecimal has no limit.
- `sys.set_int_max_str_digits` changes the limit.
