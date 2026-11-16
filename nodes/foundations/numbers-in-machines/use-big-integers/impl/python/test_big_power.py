# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-big-integers/.
import sys

import pytest

from big_power import big_power

P200 = "1606938044258990275541962092341162602522202993782792835301376"

CASES = {
    "small": (2, 10, "1024"),
    "two-to-the-100": (2, 100, "1267650600228229401496703205376"),
    "ten-to-the-30": (10, 30, "1" + "0" * 30),
    "zero-exponent": (7, 0, "1"),
    "zero-to-the-zero": (0, 0, "1"),
    "zero-base": (0, 5, "0"),
    "minus-one-odd": (-1, 199, "-1"),
    "negative-odd": (-3, 41, "-36472996377170786403"),
    "negative-even": (-3, 40, "12157665459056928801"),
    "int64-max-plus-one": (2, 63, "9223372036854775808"),
    "uint64-max-plus-one": (2, 64, "18446744073709551616"),
    "two-to-the-200": (2, 200, P200),
    "largest": (1000, 200, "1" + "0" * 600),
    "smallest": (-1000, 199, "-1" + "0" * 597),
}


@pytest.mark.parametrize("case", CASES)
def test_big_power(case):
    base, exp, want = CASES[case]
    assert big_power(base, exp) == want


def test_fixed_width():
    big = 2**63
    assert int("9223372036854775808") == big
    assert (big - 1).to_bytes(8, "big", signed=True) == b"\x7f" + b"\xff" * 7
    with pytest.raises(OverflowError):
        big.to_bytes(8, "big", signed=True)
    assert int.from_bytes(b"\x80" + b"\x00" * 7, "big", signed=True) == -big


def test_str_digit_limit():
    limit = sys.get_int_max_str_digits()
    if limit:
        with pytest.raises(ValueError):
            str(10 ** (limit + 1))
