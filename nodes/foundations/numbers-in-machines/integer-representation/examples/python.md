```python run id=bytes-and-struct
import struct

n = -1000
# int.to_bytes needs a width, a byte order and a choice of signed or unsigned.
print(n.to_bytes(2, "big", signed=True).hex(" "), n.to_bytes(2, "little", signed=True).hex(" "))
print(int.from_bytes(b"\xfc\x18", "big", signed=True), int.from_bytes(b"\xfc\x18", "big"))
try:
    (300).to_bytes(1, "big")
except OverflowError as e:
    print("OverflowError:", e)

# struct packs C types: < is little-endian, > is big-endian; h is a 16-bit short, I a 32-bit unsigned int.
print(struct.pack("<h", -2).hex(" "), struct.pack(">I", 1).hex(" "))
print(struct.unpack("<H", struct.pack("<h", -2))[0])

# Python ints have no width, so they never overflow.
print(2**64 + 1, (-7) // 2, -7 % 2)
```

```output for=bytes-and-struct
fc 18 18 fc
-1000 64536
OverflowError: int too big to convert
fe ff 00 00 00 01
65534
18446744073709551617 -4 1
```

- Verdict: native. A Python `int` has no fixed width, so it never overflows, and `2**64 + 1` is exact. To get fixed-width bytes, `int.to_bytes` takes a length, a byte order and a `signed` flag, and it raises `OverflowError` when the value does not fit. The `struct` module packs C types, with `<` for little-endian and `>` for big-endian. Integer division and `%` round toward negative infinity, so `-7 // 2` is −4.
