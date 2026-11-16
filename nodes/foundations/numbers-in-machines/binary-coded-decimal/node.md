---
tag: QBZ504
kind: concept
slug: binary-coded-decimal
name: Binary-coded decimal
aliases: [BCD, packed BCD, packed decimal, unpacked BCD, zoned decimal, COMP-3, PACKED-DECIMAL, 8421 code, NBCD]
area: foundations/numbers-in-machines
level: advanced
summary: >-
  Binary-coded decimal stores each decimal digit in its own four or eight bits. Packed BCD puts two digits in a byte, often with a sign nibble, and an addition needs a decimal adjust step.
applies_to: core-languages
viz: optional
formal: none
see_also: [binary-number, integer-representation, fixed-point-arithmetic, bits-and-bytes, ones-complement-and-sign-magnitude, floating-point-arithmetic]
same_as:
  wikidata: Q276582
  wikipedia: Binary-coded_decimal
sources:
  - url: https://en.wikipedia.org/wiki/Binary-coded_decimal
    title: Binary-coded decimal, Wikipedia
    locator: the introduction; Background; Packed BCD; Zoned decimal; Addition; Subtraction; BCD in computers; Comparison with pure binary; Application; Legal history
    accessed: 2026-11-16
  - url: https://www.felixcloutier.com/x86/daa
    title: "DAA: Decimal Adjust AL after Addition, x86 instruction reference (from the Intel SDM)"
    locator: Description; Operation
    accessed: 2026-11-16
---

## Idea

Binary-coded decimal (BCD) is "a class of binary encodings of decimal numbers where each digit is represented by a fixed number of bits, usually four or eight"[^1]. The usual code gives each digit its own 4-bit binary value, so 0 is `0000` and 9 is `1001`. Wikipedia calls this code BCD 8421 and says that it "is the most common encoding"[^1]. The six patterns from `1010` to `1111` are not valid digits.

Wikipedia names two main forms. The term unpacked BCD "usually implies a full byte for each digit (often including a sign), whereas packed BCD typically encodes two digits within a single byte"[^1]. A third form, zoned decimal, stores one digit in the low four bits of a byte and sets the high four bits so that the byte is the character code of the digit[^1].

```python run id=forms
def packed(n):
    digits = str(abs(n)) + ("D" if n < 0 else "C")
    if len(digits) % 2:
        digits = "0" + digits
    return bytes.fromhex(digits)


def zoned_ascii(n):
    return bytes(0x30 | int(d) for d in str(n))


def unpack(b):
    h = b.hex().upper()
    value = int(h[:-1])
    return -value if h[-1] == "D" else value


print("127 in 8421 BCD:", " ".join(f"{int(d):04b}" for d in "127"))
print("127 in binary:", f"{127:b}")
print("packed 127:", packed(127).hex(" ").upper())
print("packed -127:", packed(-127).hex(" ").upper())
print("packed 2024:", packed(2024).hex(" ").upper())
print("unpack 12 7D:", unpack(bytes.fromhex("127D")))
print("zoned ASCII 127:", zoned_ascii(127).hex(" ").upper())
```

```output for=forms
127 in 8421 BCD: 0001 0010 0111
127 in binary: 1111111
packed 127: 12 7C
packed -127: 12 7D
packed 2024: 02 02 4C
unpack 12 7D: -127
zoned ASCII 127: 31 32 37
```

The hexadecimal form of a packed BCD value shows the decimal digits, because each hexadecimal digit is one nibble. `C` and `D` in the last nibble are the sign.

## How it works

**The sign nibble.** In packed BCD, "The lower nibble of the rightmost byte is usually used as the sign flag"[^1]. The preferred codes are `C` for plus and `D` for minus, and `F` means unsigned[^1]. "In packed BCD, the number 127 is represented by 0001 0010 0111 1100 (127C) and −127 is represented by 0001 0010 0111 1101 (127D)."[^1] Since a byte has two nibbles and one nibble is the sign, "a word of n bytes can contain up to 2n − 1 decimal digits"[^1]. So 2024, with four digits and a sign, needs three bytes and a leading 0, as the block shows.

**Zoned decimal.** In zoned decimal, the zone bits make each byte a character. "ASCII systems use a zone value of 0011 (hex 3), giving character codes 30 to 39 (hex)."[^1] EBCDIC systems use the zone `F` for the digits, and the zone of one byte can hold the sign[^1].

**Addition.** A BCD adder first adds in binary, then corrects each digit. "Conversion of the simple sum of two digits can be done by adding 6 (that is, 16 − 10) when the five-bit result of adding a pair of digits has a value greater than 9."[^1] For example, 9 + 8 is `10001` in binary. Adding 6 gives `0001 0111`, which is 17 in BCD[^1]. The x86 `DAA` instruction does this correction for a byte: it "Adjusts the sum of two packed BCD values to create a packed BCD result"[^2]. Its rule adds 6 when the low digit is more than 9 or the auxiliary carry flag is set, and adds 60 hexadecimal when the old value was more than 99 hexadecimal or the carry flag was set[^2].

**Subtraction.** "Subtraction is done by adding the ten's complement of the subtrahend to the minuend."[^1] The ten's complement of a 3-digit number is 999 minus the number, plus 1[^1]. This is the decimal form of [[twos-complement]].

```python run id=add
def bcd_add_byte(a, b):
    s = a + b
    low_carry = (a & 0x0F) + (b & 0x0F) > 0x0F
    if (s & 0x0F) > 9 or low_carry:
        s += 0x06
    if s > 0x99:
        s += 0x60
    return s & 0xFF, s > 0xFF


for a, b in ((0x09, 0x08), (0x45, 0x38), (0x99, 0x01)):
    r, carry = bcd_add_byte(a, b)
    print(f"{a:02X} + {b:02X} = {r:02X} carry {'yes' if carry else 'no'}")
```

```output for=add
09 + 08 = 17 carry no
45 + 38 = 83 carry no
99 + 01 = 00 carry yes
```

The function follows the rule of `DAA` for two packed BCD bytes, but it tests the sum in place of the flags of the processor.

## In languages

| Language | Verdict | BCD support |
|---|---|---|
| Python | idiom | `bytes.fromhex` and `hex` on packed digits |
| JavaScript | idiom | bit operations on numbers |
| TypeScript | idiom | as JavaScript |
| Java | idiom | bit operations on `byte` and `int` |
| Kotlin | idiom | bit operations, as Java |
| C# | idiom | bit operations on `byte` and `int` |
| Go | idiom | bit operations on `byte` |
| Rust | idiom | bit operations on `u8` |
| C | idiom | bit operations on `unsigned char` |
| C++ | idiom | bit operations on `std::uint8_t` |
| Bash | idiom | `printf` with `%x` and arithmetic expansion |
| PowerShell | idiom | bit operations on `[byte]` |
| SQL | absent | no BCD type in the standard |
| Regex | n/a | a pattern can check digits in text but does no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | n/a | no arithmetic |

None of the core languages has a BCD type, so a program packs and unpacks the nibbles with shifts and masks. The data comes from outside the program: from a file of a mainframe system, from a database, or from a device. In COBOL, packed BCD is the data type `COMPUTATIONAL-3`, "an IBM extension adopted by many other compiler vendors", or `PACKED-DECIMAL`, "part of the 1985 COBOL standard"[^1], and PL/I has it as `FIXED DECIMAL`[^1].

## Variations and related ideas

- **Other weights.** Other codes use the bit weights 4221 and 7421[^1], and there is excess-3, where each digit is stored as the digit plus 3[^1].
- **Ten's complement.** In place of a sign nibble, "Ten's complement representations for negative numbers offer an alternative approach to encoding the sign of packed (and other) BCD numbers."[^1]
- **Densely packed decimal.** Densely packed decimal stores three digits in ten bits, and it is used in one of the two decimal encodings of IEEE 754-2008[^1].
- **Decimal floating point.** Some calculators "employ a floating-point BCD format, typically with two or three digits for the (decimal) exponent"[^1]. See [[floating-point-arithmetic]].

## Pitfalls

- **Hexadecimal is not decimal.** A packed byte `0x45` means 45, but its binary value is 69. A program that reads a BCD field as a binary integer gets a wrong value with no error.
- **Invalid nibbles.** The patterns `A` to `F` are not digits, except as a sign. A decoder must check each nibble, or bad data passes as a strange number.
- **Plain binary addition.** `0x09 + 0x08` is `0x11` in binary, which reads as 11 in BCD. The result needs the add-6 correction.
- **A sign nibble in the wrong place.** The sign is in the last nibble, and a value with an even number of digits needs a leading 0 nibble. Code that forgets this shifts every digit by one place.
- **x86-64.** The `DAA` instruction "is not valid in 64-bit mode"[^2], so 64-bit code does the BCD correction in software.

## Cost

Standard BCD "requires four bits per digit, roughly 20% more space than a binary encoding"[^1]. "Practical existing implementations of BCD are typically slower than operations on binary representations, especially on embedded systems, due to limited processor support for native BCD operations."[^1] On the other hand, conversion to text "is a simple per-digit mapping, and can be done in linear (O(n)) time"[^1], and "Scaling by a power of 10 is simple."[^1] A decimal fraction such as 0.2 has a finite form in BCD but not in binary[^1].

## History

"BCD was used in many early decimal computers, and is implemented in the instruction set of machines such as the IBM System/360 series and its descendants, Digital Equipment Corporation's VAX, the Burroughs B1700, and the Motorola 68000-series processors."[^1] "Packed BCD has been in use since at least the 1960s and is implemented in all IBM mainframe hardware since then."[^1] The Intel 8080, the Zilog Z80 and the x86 family have the `DAA` instruction[^1], and the BIOS of many personal computers stores the date and time in BCD because of the real-time clock chip of the IBM PC AT[^1]. "In the 1972 case Gottschalk v. Benson, the U.S. Supreme Court overturned a lower court's decision that had allowed a patent for converting BCD-encoded numbers to binary on a computer."[^1]

## References
