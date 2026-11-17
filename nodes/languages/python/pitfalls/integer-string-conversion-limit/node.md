---
tag: CQ9S2V
kind: feature
owner: python
slug: integer-string-conversion-limit
name: Integer string conversion limit
aliases: [int max str digits, Exceeds the limit (4300 digits), sys.set_int_max_str_digits, PYTHONINTMAXSTRDIGITS, CVE-2020-10735, big int to str ValueError]
area: languages/python/pitfalls
level: advanced
summary: >-
  CPython raises ValueError when it converts an int of more than 4300 decimal digits to or from a string. The limit stops a denial of service. Raise it with sys.set_int_max_str_digits, or use hex.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/stdtypes.html#integer-string-conversion-length-limitation", locator: "Integer string conversion length limitation"}
proposals: []
see_also: [python/int-type, python/type-conversions, python/repr-and-str, python/numeric-literals, python/development-mode-and-x-options, python/saving-structured-data-with-json, python/subinterpreters]
sources:
  - url: https://docs.python.org/3.15/library/stdtypes.html#integer-string-conversion-length-limitation
    title: Built-in Types, Python 3.15 documentation
    locator: Integer string conversion length limitation; Affected APIs; Configuring the limit
    accessed: 2026-11-18
  - url: https://www.cve.org/CVERecord?id=CVE-2020-10735
    title: CVE-2020-10735, CVE Program
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/sys.html#sys.set_int_max_str_digits
    title: sys, System-specific parameters and functions, Python 3.15 documentation
    locator: sys.set_int_max_str_digits()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.11.html#other-cpython-implementation-changes
    title: What’s New In Python 3.11
    locator: Other CPython Implementation Changes
    accessed: 2026-11-18
---

## Synopsis

```python fails id=too-many-digits expect=exception:ValueError
import sys

sys.set_int_max_str_digits(4300)
n = int("2" * 4300)
print(len(str(n)))

big = int("2" * 5432)
```

A string of 4300 digits converts. A string of 5432 digits raises `ValueError: Exceeds the limit (4300 digits) for integer string conversion: value has 5432 digits; use sys.set_int_max_str_digits() to increase the limit`.

## Semantics

**r1.** "CPython has a global limit for converting between int and str to mitigate denial of service attacks. This limit only applies to decimal or other non-power-of-two number bases. Hexadecimal, octal, and binary conversions are unlimited."[^1]

**r2.** The reason is the cost. "Converting a large value such as int('1' * 500_000) can take over a second on a fast CPU."[^1] The CVE record says that "a system could take 50ms to parse an int string with 100,000 digits and 5s for 1,000,000 digits" and that "The highest threat from this vulnerability is to system availability."[^2] "Limiting conversion size offers a practical way to avoid CVE 2020-10735."[^1]

**r3.** The size: "The default limit is 4300 digits as provided in sys.int_info.default_max_str_digits. The lowest limit that can be configured is 640 digits".[^1] The limit counts digits only: "Underscores and the sign are not counted towards the limit."[^1]

**r4.** The limit applies in both directions, so it can fail far from the input. A value that was read can grow, and `str` on the result fails. Example: [`square-fails`](#square-fails). The affected conversions include `int(string)` with base 10, `int(string, base)` for a base that is not a power of 2, `str(integer)`, `repr(integer)`, and "any other string conversion to base 10, for example f"{integer}", "{}".format(integer), or b"%d" % integer."[^1]

**r5.** Some conversions have no limit: "int(string, base) with base 2, 4, 8, 16, or 32", "int.from_bytes() and int.to_bytes()", "hex(), oct(), bin()", "str to float" and "str to decimal.Decimal".[^1] Example: [`hex-unlimited`](#hex-unlimited).

**r6.** To change the limit before startup, set `PYTHONINTMAXSTRDIGITS` or `-X int_max_str_digits`. A value of 0 turns the limit off.[^1] From code, use `sys.get_int_max_str_digits()` and `sys.set_int_max_str_digits()`. The setter will "Set the integer string conversion length limitation used by this interpreter."[^3] "Subinterpreters have their own limit."[^1] Example: [`raise-limit`](#raise-limit).

**r7.** Do not turn the limit off for input from users or the network. The limit protects code that reads numbers from text, such as a JSON parser (rule r2). Raise it only as far as the program needs, and only for data that you trust.

## Examples

```python run id=raise-limit
import sys

sys.set_int_max_str_digits(4300)
print(sys.get_int_max_str_digits(), sys.int_info.default_max_str_digits)

sys.set_int_max_str_digits(10_000)
n = int("7" * 6000)
print(len(str(n)))

sys.set_int_max_str_digits(4300)
```

```output for=raise-limit
4300 4300
6000
```

After the call, the interpreter accepts up to 10 000 digits (rule r6). The last line puts the default back.

```python run id=hex-unlimited
import sys

sys.set_int_max_str_digits(4300)
n = 1 << 20_000
text = hex(n)
print(len(text), int(text, 16) == n)
print(n.bit_length(), len(n.to_bytes(2501, "big")))
```

```output for=hex-unlimited
5003 True
20001 2501
```

The number has 6021 decimal digits, so `str(n)` would fail. `hex`, `int(..., 16)` and `to_bytes` have no limit (rule r5).

## Versions

> [!since v="3.11"]
> The limit is new. "Converting between int and str in bases other than 2 (binary), 4, 8 (octal), 16 (hexadecimal), or 32 such as base 10 (decimal) now raises a ValueError if the number of digits in string form is above a limit to avoid potential denial of service attacks due to the algorithmic complexity. This is a mitigation for CVE 2020-10735."[^4] "The default limit is 4300 digits in string form."[^4] The documentation also says that "these APIs were added in security patch releases in versions before 3.12."[^1]

## Pitfalls

```python fails id=square-fails expect=exception:ValueError
import sys

sys.set_int_max_str_digits(4300)
i = int("2" * 4300)
print(len(str(i)))

i_squared = i * i
print(len(str(i_squared)))
```

The input had 4300 digits and converted. Its square has about 8600 digits, so `str` fails on the output, not on the input (rule r4).

```python fails id=json-big-number expect=exception:ValueError
import json
import sys

sys.set_int_max_str_digits(4300)
payload = '{"id": ' + "9" * 5000 + "}"
data = json.loads(payload)
```

The JSON parser converts the number text with `int`, so a long number in the input raises `ValueError` (rule r4). This is the protection of rule r7. Catch the error, and reject the input.

## Comparison

In Python, `int` has no fixed size, so a number from text can have any number of digits. "There exists no algorithm that can convert a string to a binary integer or a binary integer to a string in linear time, unless the base is a power of 2."[^1] So the cost grows faster than the length (rule r2), so CPython sets a limit on the length (rule r1). A program that needs to store very large numbers as text can use hexadecimal or bytes (rule r5). For the type, see [[python/int-type]].
