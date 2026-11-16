```python run id=bytes
raw = bytes([0xFF, 0xFE])
print("unsigned:", int.from_bytes(raw, "big"))
print("signed:", int.from_bytes(raw, "big", signed=True))
print("-1 as 32-bit unsigned:", -1 & 0xFFFF_FFFF)
print("-2 to 2 bytes, signed:", (-2).to_bytes(2, "big", signed=True).hex())
try:
    (-2).to_bytes(2, "big")
except OverflowError:
    print("-2 to 2 bytes, unsigned: OverflowError")
```

```output for=bytes
unsigned: 65534
signed: -2
-1 as 32-bit unsigned: 4294967295
-2 to 2 bytes, signed: fffe
-2 to 2 bytes, unsigned: OverflowError
```

- Verdict: absent. Python has one integer type, and it has no width and no unsigned form.
- Signedness matters only at the edge of the program, where bytes come in or go out. `from_bytes` and `to_bytes` take `signed=True` for two's complement.
- `x & 0xFFFF_FFFF` gives the unsigned 32-bit value of a negative number.
- `to_bytes` without `signed=True` refuses a negative number with `OverflowError`.
