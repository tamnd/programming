---
tag: 27NFBE
kind: concept
slug: binary-number
name: Binary number
aliases: [binary number, binary, base 2, binary numeral system, binary literal, bit string, hexadecimal, octal, radix, number base, positional notation]
area: foundations/numbers-in-machines
level: core
summary: >-
  A binary number writes a value in base 2, with the digits 0 and 1. Computers store integers this way, and hexadecimal and octal are short forms of the same bits.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1703
  people: [Gottfried Wilhelm Leibniz, George Boole, Claude Shannon, George Stibitz, Konrad Zuse, John von Neumann]
  note: Leibniz published binary arithmetic in 1703. Shannon and Stibitz showed in 1937 that relay circuits can add binary numbers, and from the EDVAC report of 1945 most computers used binary.
see_also: [bits-and-bytes, integer-overflow, floating-point-arithmetic, parse-an-integer, memory-address, machine-code, processor-and-memory, base64-encode-and-decode]
same_as:
  wikidata: Q3913
  wikipedia: Binary_number
sources:
- url: https://docs.python.org/3/library/functions.html
  title: Built-in Functions, Python documentation
  locator: bin() and int()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/toString
  title: Number.prototype.toString(), MDN Web Docs
  locator: Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Integer.html
  title: Class Integer, Java SE 25 API documentation
  locator: toBinaryString(int)
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings
  title: Standard numeric format strings, .NET documentation
  locator: Binary format specifier (B)
- url: https://pkg.go.dev/strconv
  title: Package strconv, Go standard library documentation
  locator: ParseInt and FormatInt
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Integer literals
- url: https://doc.rust-lang.org/std/fmt/trait.Binary.html
  title: Trait Binary, Rust standard library documentation
  locator: Description
- url: https://en.cppreference.com/w/c/string/byte/strtol.html
  title: strtol, strtoll, C reference, cppreference.com
  locator: Description
- url: https://en.cppreference.com/w/cpp/utility/bitset.html
  title: std::bitset, C++ reference, cppreference.com
  locator: Description
---

## Idea

In decimal, each digit is worth ten times the digit to its right. In binary, each digit is worth two times the digit to its right, and the only digits are 0 and 1. So `101101` means 1×32 + 0×16 + 1×8 + 1×4 + 0×2 + 1×1, which is 45. A binary digit is a bit, and a switch, a charge or a magnetic spot can hold one, so computers store and add numbers in binary. People rarely write long binary numbers; they write the same bits in hexadecimal, base 16, where each digit stands for exactly 4 bits.

```python run id=idea
def to_binary(n):
    """Repeated division by 2: each remainder is the next bit, from the lowest."""
    bits = ""
    while n > 0:
        n, r = divmod(n, 2)
        bits = str(r) + bits
    return bits or "0"


def from_binary(bits):
    """Each step doubles the value so far and adds the next bit."""
    n = 0
    for b in bits:
        n = 2 * n + int(b)
    return n


n = 45
b = to_binary(n)
print(n, "->", b)
print(" + ".join(f"{d}*2^{i}" for i, d in reversed(list(enumerate(reversed(b)))) if d == "1"), "=", from_binary(b))
print("built-ins:", bin(n), int("101101", 2), f"{n:08b}")

# Binary addition: add bit by bit from the right, with a carry.
x, y = "1011", "0110"
carry, out = 0, ""
for a, c in zip(reversed(x), reversed(y)):
    s = int(a) + int(c) + carry
    out = str(s % 2) + out
    carry = s // 2
out = ("1" if carry else "") + out
print(f"{x} + {y} = {out}  ({from_binary(x)} + {from_binary(y)} = {from_binary(out)})")
```

```output for=idea
45 -> 101101
1*2^5 + 1*2^3 + 1*2^2 + 1*2^0 = 45
built-ins: 0b101101 45 00101101
1011 + 0110 = 10001  (11 + 6 = 17)
```

`to_binary` divides by 2 again and again, and the remainders are the bits from the lowest to the highest. `from_binary` goes the other way: for each bit, it doubles the value so far and adds the bit. The sum on the second line shows the place value of each 1 bit. The built-ins do the same work: `bin` adds the prefix `0b`, `int` with base 2 parses, and the format `08b` pads to 8 digits. The addition works like decimal addition by hand, with a carry of 1 when a column adds up to 2 or 3.

## How it works

**Place values.** A number in base *b* with digits *dₙ … d₁ d₀* has the value *dₙ·bⁿ + … + d₁·b + d₀*. In base 2 the place values are 1, 2, 4, 8, 16 and so on, so *n* bits can hold the values from 0 to 2ⁿ − 1: 8 bits hold 0 to 255, 32 bits hold 0 to 4,294,967,295. The number of bits that a value needs is its bit length, ⌊log₂ *n*⌋ + 1 for *n* > 0, so 45 needs 6 bits and 2¹⁰⁰ needs 101.

**Conversion.** To write *n* in base 2, divide by 2 and keep the remainder until the quotient is 0, then read the remainders backward. To read a binary number, start with 0 and, for each bit from the left, double and add the bit. The same two loops work for any base, which is why library functions take the base as an argument. JavaScript's `toString` takes a radix, "An integer in the range 2 through 36", and uses the letters a to z for digits above 9.[^2]

**Hexadecimal and octal.** 16 is 2⁴, so each hexadecimal digit is exactly 4 bits, and two digits are one byte: `0xAC` is `1010 1100`. 8 is 2³, so each octal digit is 3 bits. This makes the conversion between them and binary a matter of grouping the bits, with no arithmetic, and it is why programmers write masks, colours, addresses and byte values in hexadecimal. Octal survives in Unix file permissions, where `755` is the three groups of bits `111 101 101`.

**Literals and conversion in languages.** Most languages accept integer literals in several bases with a prefix: `0b` or `0B` for binary, `0o` or `0` for octal and `0x` for hexadecimal. The Go specification says: "An optional prefix sets a non-decimal base: 0b or 0B for binary, 0, 0o, or 0O for octal, and 0x or 0X for hexadecimal".[^6] Most also allow a separator, `_` or `'` in C++, to group digits. Functions convert in both directions: Python's `bin` converts "an integer number to a binary string prefixed with "0b"",[^1] C# has the format specifier `B`, which "converts a number to a string of binary digits",[^4] Go's `strconv.ParseInt` takes a base,[^5] and C's `strtol` takes a base from 2 to 36 and stops at the first character that is not a digit of that base.[^8]

**Negative numbers.** A binary number with a minus sign, such as `-101`, is how mathematics writes negative values, and Python prints `bin(-5)` that way because its integers have no fixed width. Machines store negative integers in a fixed number of bits, almost always in two's complement, where −5 in 8 bits is `11111011`. Functions that print the bits of a fixed-width integer show this form: Java's `Integer.toBinaryString` gives "a string representation of the integer argument as an unsigned integer in base 2",[^3] and in Rust, "negative values are formatted as the two's complement representation".[^7]

**Fractions.** Binary also has a point: `0.1` in binary is one half, and `0.011` is three eighths. A fraction whose denominator is not a power of 2, such as one tenth, has no finite binary form, just as one third has no finite decimal form. This is why `0.1 + 0.2` is not exactly `0.3` in [floating-point arithmetic](/floating-point-arithmetic/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `0b` literals; `bin`, `int(s, 2)`, format `b`; `int.bit_length`, `int.bit_count` |
| JavaScript | native | `0b` literals; `toString(2)`, `parseInt(s, 2)`; `BigInt` |
| TypeScript | native | as JavaScript |
| Java | native | `0b` literals; `Integer.toBinaryString`, `Integer.parseInt(s, 2)` |
| Kotlin | native | `0b` literals; `toString(2)`, `toInt(2)` |
| C# | native | `0b` literals; format `B`; `Convert.ToString(x, 2)` |
| Go | native | `0b` literals; `%b`; `strconv.FormatInt`, `strconv.ParseInt` |
| Rust | native | `0b` literals; `{:b}`; `from_str_radix` |
| C | native | `0b` literals in recent standards; `strtol` with base 2 |
| C++ | native | `0b` literals; `std::bitset`; `std::format` with `b`; `std::from_chars` |
| Bash | native | `2#101101` in arithmetic; `$((16#ff))` |
| PowerShell | native | `0b` literals; `[Convert]::ToString(x, 2)` |
| SQL | native | `0b`, `0o` and `0x` integer literals in the current standard and PostgreSQL; bit-string types |
| Regex | n/a | `[01]+` matches binary digits as text |
| HTML | n/a | no numbers |
| CSS | n/a | hexadecimal colours such as `#ff8000`, no binary |

All the general-purpose languages in the table write binary literals and convert numbers to and from binary text, so the verdict is native. They differ in how they show negative numbers: Python and JavaScript print a minus sign, while Java, C#, Go's unsigned formats, Rust and C++ show the two's complement bits at the width of the type. Bash reads numbers in any base from 2 to 64 with the form `base#digits`.

The example files show literals, printing, parsing and bit counts in each language. Python shows a negative number with a sign, and masks it to get its 8 bits. JavaScript shows the limit of exact integers in a 64-bit float and the 32-bit bitwise operators. Java and C# print negative numbers as 32 bits of two's complement. Go reports an error for a digit that is not binary. Rust formats negative values at their own width. C writes its own printing function and shows where `strtol` stops. C++ uses `std::bitset`, which "represents a fixed-size sequence of N bits",[^9] and the functions of `<bit>`.

## Variations and related ideas

- **Bits of fixed width.** Machine integers have 8, 16, 32 or 64 bits, and arithmetic wraps around or overflows at the top. See [integer overflow](/integer-overflow/) and [bits and bytes](/bits-and-bytes/).
- **Binary-coded decimal.** BCD stores each decimal digit in 4 bits, which keeps decimal fractions exact; old business machines, some calculators and the x86 instruction set support it.
- **Gray code.** A binary order in which two neighbouring values differ in one bit, used in rotary encoders and Karnaugh maps.
- **Balanced ternary and other bases.** The Soviet Setun computer of 1958 used base 3 with the digits −1, 0 and 1. Base 64 and base 32 encode bytes as text. See [Base64](/base64-encode-and-decode/).
- **Bit sets.** An integer or an array of bits can hold a set of small numbers, with one bit per member, and bitwise operators give union and intersection.

## Pitfalls

- **Leading zeros in literals.** In C, C++, Java and JavaScript in sloppy mode, a literal such as `0755` is octal, so `010` is 8. Python 3 and strict-mode JavaScript reject it. Use `0o` where the language has it.
- **Signed and unsigned output.** The same 32 bits print as `-5` or `4294967291` depending on the type, and binary output functions differ: Python prints `-0b101`, Java prints 32 bits. Say which form a protocol or file uses.
- **Parsing that stops early.** `strtol`, `parseInt` and similar functions stop at the first character that is not a digit and return what they read, so `parseInt("102", 2)` is 2. Check that the whole string was used, or use a function that reports an error, such as Go's `strconv.ParseInt` or Rust's `from_str_radix`.
- **Overflow in parsing.** A long binary string does not fit in a 32-bit integer. Use a wider type, an unsigned parse or a big integer type.
- **Decimal fractions.** Money and other decimal amounts are not exact in binary floating point. Store them in integer units, such as cents, or in a decimal type.

## Cost

Converting a number of *n* bits to or from binary text takes O(*n*) steps for machine integers, and printing is cheap next to the cost of output. For big integers, the simple conversion to decimal takes O(*n*²) time, because each division by 10 touches all the digits; libraries use divide-and-conquer methods for very large numbers, and conversion to binary or hexadecimal stays linear because those bases are powers of 2. Counting bits, finding the highest bit and finding the lowest bit are single instructions on most processors, which the functions `bit_count`, `popcount`, `count_ones` and `leading_zeros` use.

## History

Gottfried Wilhelm Leibniz published *Explication de l'Arithmétique Binaire* in 1703, with tables for addition and multiplication in base 2. George Boole's algebra of logic, from 1854, used the two values true and false. In 1937, Claude Shannon showed in his master's thesis that relay circuits compute Boolean algebra, and George Stibitz built a binary adder from relays on his kitchen table. Konrad Zuse's Z1 of 1938 and his later machines used binary.

The ENIAC of 1945 worked in decimal, but John von Neumann's EDVAC report of the same year argued for binary, and most later computers followed it. Programmers wrote octal on machines with words of 12, 18 or 36 bits, such as the PDP-8 and the IBM 7090. IBM's System/360 of 1964 used 8-bit bytes and documented its machine in hexadecimal, which became the usual form for bytes and addresses.

## References
