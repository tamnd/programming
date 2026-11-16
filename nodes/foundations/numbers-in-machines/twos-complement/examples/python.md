```python run id=infinite-sign
print("~5:", ~5)
print("-6 & 0xFF:", f"{-6 & 0xFF:08b}")
print("(-6).to_bytes(2, signed=True):", (-6).to_bytes(2, "big", signed=True).hex())
print("from_bytes(b'\\xfa', signed=True):", int.from_bytes(b"\xfa", "big", signed=True))
print("-(-128):", -(-128))
```

```output for=infinite-sign
~5: -6
-6 & 0xFF: 11111010
(-6).to_bytes(2, signed=True): fffa
from_bytes(b'\xfa', signed=True): -6
-(-128): 128
```

- Verdict: native. Python's `int` has no width, but bitwise operations act as if it were two's complement with an infinite number of sign bits.
- `x & 0xFF` gives the low 8 bits of the two's complement form. `to_bytes` with `signed=True` writes the form in a fixed number of bytes, and the 2-byte form of -6 shows the sign extension.
- `-(-128)` is 128, because Python integers do not overflow.
