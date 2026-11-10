# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-an-integer/.

import pytest

from parse_integer import parse_integer

CASES = {
    "decimal": ("42", 10, 42),
    "negative": ("-17", 10, -17),
    "hex": ("ff", 16, 255),
    "binary": ("101010", 2, 42),
    "base-36": ("zz", 36, 1295),
    "plus-sign": ("+5", 10, 5),
    "negative-zero": ("-0", 10, 0),
    "zero": ("0", 10, 0),
    "leading-zeros": ("007", 10, 7),
    "many-leading-zeros": ("0" * 5000 + "42", 10, 42),
    "max": ("9223372036854775807", 10, 9223372036854775807),
    "min": ("-9223372036854775808", 10, -9223372036854775808),
    "max-hex": ("7fffffffffffffff", 16, 9223372036854775807),
    "min-binary": ("-1" + "0" * 63, 2, -9223372036854775808),
    "upper-case": ("FF", 16, 255),
    "mixed-case": ("Zz", 36, 1295),
    "above-safe-integer": ("9007199254740993", 10, 9007199254740993),
}

INVALID = {
    "empty": ("", 10),
    "sign-only": ("-", 10),
    "plus-only": ("+", 10),
    "double-sign": ("+-5", 10),
    "leading-space": (" 42", 10),
    "trailing-space": ("42 ", 10),
    "trailing-newline": ("42\n", 10),
    "trailing-garbage": ("42px", 10),
    "underscore": ("1_000", 10),
    "thousands-separator": ("1,000", 10),
    "decimal-point": ("4.0", 10),
    "exponent": ("1e3", 10),
    "hex-prefix": ("0x1A", 16),
    "binary-prefix": ("0b101", 2),
    "digit-outside-base": ("102", 2),
    "letter-outside-base": ("fg", 16),
    "arabic-indic-digits": ("\u0661\u0662", 10),
    "fullwidth-digits": ("\uff11\uff12", 10),
    "kelvin-sign": ("\u212a", 36),
}

OUT_OF_RANGE = {
    "max-plus-one": ("9223372036854775808", 10),
    "min-minus-one": ("-9223372036854775809", 10),
    "two-to-the-64": ("18446744073709551616", 10),
    "hex-sign-bit": ("8000000000000000", 16),
    "hex-all-ones": ("ffffffffffffffff", 16),
    "min-times-ten": ("-92233720368547758080", 10),
    "many-digits": ("9" * 5000, 10),
}


@pytest.mark.parametrize("name", CASES)
def test_parse_integer(name):
    s, base, want = CASES[name]
    assert parse_integer(s, base) == want


@pytest.mark.parametrize("name", INVALID)
def test_invalid(name):
    s, base = INVALID[name]
    with pytest.raises(ValueError):
        parse_integer(s, base)


@pytest.mark.parametrize("name", OUT_OF_RANGE)
def test_out_of_range(name):
    s, base = OUT_OF_RANGE[name]
    with pytest.raises(OverflowError):
        parse_integer(s, base)
