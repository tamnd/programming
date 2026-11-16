```python run id=packed
def to_packed(n):
    digits = [int(d) for d in str(abs(n))] + [0xD if n < 0 else 0xC]
    if len(digits) % 2:
        digits.insert(0, 0)
    return bytes(digits[i] << 4 | digits[i + 1] for i in range(0, len(digits), 2))


def from_packed(b):
    nibbles = [x for byte in b for x in (byte >> 4, byte & 0x0F)]
    *digits, sign = nibbles
    if any(d > 9 for d in digits) or sign not in (0xC, 0xD, 0xF):
        raise ValueError("not packed BCD")
    value = int("".join(map(str, digits)))
    return -value if sign == 0xD else value


p = to_packed(-12345)
print("packed -12345:", p.hex(" ").upper())
print("back:", from_packed(p))
print("0x45 read as binary:", 0x45)
print("0x45 read as BCD:", (0x45 >> 4) * 10 + (0x45 & 0x0F))
try:
    from_packed(bytes.fromhex("1A3C"))
except ValueError as e:
    print("1A 3C:", e)
```

```output for=packed
packed -12345: 12 34 5D
back: -12345
0x45 read as binary: 69
0x45 read as BCD: 45
1A 3C: not packed BCD
```

- Verdict: idiom. Python has no BCD type. The block packs two nibbles in each byte with shifts and masks, and `bytes.hex` shows the digits.
- The decoder checks each digit nibble and the sign nibble, so the invalid nibble `A` raises an error.
