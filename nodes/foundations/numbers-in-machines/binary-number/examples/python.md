```python run id=literals-and-arbitrary-width
n = 0b1010_1100  # binary literal; the underscore only groups digits
print(n, oct(n), hex(n), bin(n))
print(int("ff", 16), int("0b1010", 0), int("777", 8))
print(f"{5:b} {5:08b} {255:#x} {1_000_000:_b}")

# Python integers have no fixed width, so a negative number shows a minus sign.
print(bin(-5), (-5).bit_length(), (2**100).bit_length())
# Masking with 0xFF gives the low 8 bits, as an 8-bit register would hold them.
print(f"{-5 & 0xFF:08b}", (0b1011).bit_count())
```

```output for=literals-and-arbitrary-width
172 0o254 0xac 0b10101100
255 10 511
101 00000101 0xff 1111_0100_0010_0100_0000
-0b101 3 101
11111011 3
```

- Verdict: native. Python writes literals in base 2, 8 and 16, and `int` parses any base from 2 to 36; base 0 reads the prefix from the text. The format `_b` groups binary digits by four. Python integers have no fixed width, so `bin(-5)` has a minus sign, and `& 0xFF` gives the 8 bits that a byte would hold. `bit_length` and `bit_count` give the number of bits needed and the number of 1 bits.
