```python run id=tiny
import math
import struct
import sys


def fields(x):
    b = struct.unpack(">Q", struct.pack(">d", x))[0]
    return f"{b >> 63} {(b >> 52) & 0x7FF:011b} {b & ((1 << 52) - 1):013x}"


def is_subnormal(x):
    return math.isfinite(x) and x != 0 and abs(x) < sys.float_info.min


def yes(b):
    return "yes" if b else "no"


tiny = sys.float_info.min
for name, v in [("min normal", tiny), ("min normal / 2", tiny / 2), ("ulp(0.0)", math.ulp(0.0))]:
    print(f"{name:15} {fields(v)}")

print("subnormal 1e-310:", yes(is_subnormal(1e-310)))
print("subnormal 1e-300:", yes(is_subnormal(1e-300)))
print("subnormal 0.0:   ", yes(is_subnormal(0.0)))

third = 1 / 3
trip = third * 2.0**-1070 * 2.0**1000 * 2.0**70
print("1/3:             ", third)
print("1/3 after a trip:", trip)
```

```output for=tiny
min normal      0 00000000001 0000000000000
min normal / 2  0 00000000000 8000000000000
ulp(0.0)        0 00000000000 0000000000001
subnormal 1e-310: yes
subnormal 1e-300: no
subnormal 0.0:    no
1/3:              0.3333333333333333
1/3 after a trip: 0.3125
```

- Verdict: native. Python `float` is an IEEE 754 double with subnormals.
- The fields are the sign, the exponent field and the fraction in hex. The smallest normal has an exponent field of 1 and a fraction of 0. A subnormal has an exponent field of 0.
- `sys.float_info.min` is the smallest normal value and `math.ulp(0.0)` is the smallest subnormal. Python 3.15 has `math.issubnormal`. The `is_subnormal` here does the same test on older versions.
- `1/3` scaled down into the subnormal range keeps only a few bits. Scaled back up, it is a different number.
