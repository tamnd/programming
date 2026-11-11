```python run id=flags-shifts-and-bytes
import struct

READ, WRITE, EXEC = 1 << 0, 1 << 1, 1 << 2
perm = READ | WRITE
print(bool(perm & WRITE), bool(perm & EXEC))  # test a bit
perm &= ~WRITE  # clear a bit
perm ^= EXEC  # toggle a bit
print(f"{perm:03b}", perm.bit_count(), perm.bit_length())

print(~5, -8 >> 1, -1 & 0xFF)  # as if there were infinite sign bits
print(1 << 100)  # ints have no fixed width

n = 0x12345678
print(n.to_bytes(4, "little").hex(), struct.pack(">I", n).hex())
print(hex(int.from_bytes(b"\x78\x56\x34\x12", "little")))
```

```output for=flags-shifts-and-bytes
True False
101 2 3
-6 -4 255
1267650600228229401496703205376
78563412 12345678
0x12345678
```

```python wrong id=invert-without-mask
b = 0b0000_1111
inverted = ~b  # meant to flip the 8 bits of a byte
print(inverted, f"{inverted:b}")
try:
    bytes([inverted])
except ValueError as e:
    print(e)
print(~b & 0xFF)  # mask to the width you mean
```

```output for=invert-without-mask
-16 -10000
bytes must be in range(0, 256)
240
```

- Verdict: native. Python has the operators `&`, `|`, `^`, `~`, `<<` and `>>` on `int`, and `bin`, `hex` and format specs such as `:08b` to print bits. An `int` has no fixed width, so the operators behave as if a negative number had an infinite row of 1 bits on the left: `~5` is `-6` and `-8 >> 1` is `-4`. `bit_count` and `bit_length` count bits, and `int.to_bytes`, `int.from_bytes` and the `struct` module convert between integers and bytes in a stated byte order.
- The wrong block flips the bits of a byte with `~`. With no fixed width, the result is the negative number `-16`, not `240`, and `bytes` rejects it. Mask the result to the width you mean, as with `& 0xFF`.
