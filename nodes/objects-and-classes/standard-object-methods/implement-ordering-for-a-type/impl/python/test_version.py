# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

import pytest

from version import Version, sort_versions

CASES = {
    "simple": (["1.2.0", "1.0.0", "1.1.0"], ["1.0.0", "1.1.0", "1.2.0"]),
    "fields-in-order": (["2.0.0", "1.9.9", "1.10.0"], ["1.9.9", "1.10.0", "2.0.0"]),
    "patch": (["0.1.10", "0.1.2", "0.1.1"], ["0.1.1", "0.1.2", "0.1.10"]),
    "empty": ([], []),
    "single": (["3.4.5"], ["3.4.5"]),
    "numeric-not-text": (["10.0.0", "9.0.0", "1.0.0"], ["1.0.0", "9.0.0", "10.0.0"]),
    "duplicates": (["1.0.0", "0.9.0", "1.0.0"], ["0.9.0", "1.0.0", "1.0.0"]),
    "zero": (["0.0.1", "0.0.0"], ["0.0.0", "0.0.1"]),
    "limit": (["1000000.0.0", "999999.1000000.1000000"], ["999999.1000000.1000000", "1000000.0.0"]),
}


@pytest.mark.parametrize("name", CASES)
def test_sort_versions(name):
    versions, want = CASES[name]
    assert sort_versions(versions) == want


def test_order_rules():
    a, b, c = Version(1, 2, 3), Version(1, 10, 0), Version(2, 0, 0)
    assert a < b < c and a < c
    assert not a < a and a <= a and a >= a
    assert b > a and not b < a
    assert Version(1, 2, 3) == a and hash(Version(1, 2, 3)) == hash(a)
    assert max([a, c, b]) is c


def test_other_types_are_not_ordered():
    with pytest.raises(TypeError):
        Version(1, 0, 0) < "1.0.0"
