# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-number-with-separators/.

import pytest

from group import group_thousands

CASES = [
    ("million", 1234567, "1,234,567"),
    ("thousand", 1000, "1,000"),
    ("small", 42, "42"),
    ("zero", 0, "0"),
    ("three-digits", 999, "999"),
    ("negative", -1234567, "-1,234,567"),
    ("negative-small", -999, "-999"),
    ("negative-thousand", -1000, "-1,000"),
    ("int64-max", 9223372036854775807, "9,223,372,036,854,775,807"),
    ("int64-min", -9223372036854775808, "-9,223,372,036,854,775,808"),
    ("six-digits", 100000, "100,000"),
    ("seven-digits", 1000000, "1,000,000"),
]


@pytest.mark.parametrize(("name", "n", "want"), CASES, ids=[c[0] for c in CASES])
def test_group_thousands(name, n, want):
    assert group_thousands(n) == want
