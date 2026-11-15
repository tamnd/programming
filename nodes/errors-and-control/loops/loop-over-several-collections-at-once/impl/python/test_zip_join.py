# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

import pytest

from zip_join import zip_join

CASES = {
    "rosetta": ((["a", "b", "c"], ["A", "B", "C"], [1, 2, 3]), ["aA1", "bB2", "cC3"]),
    "words": ((["x", "y"], ["X", "Y"], [10, 20]), ["xX10", "yY20"]),
    "shorter-first": ((["a"], ["A", "B"], [1, 2, 3]), ["aA1"]),
    "shorter-last": ((["a", "b", "c"], ["A", "B", "C"], [1]), ["aA1"]),
    "one-empty": ((["a", "b"], [], [1, 2]), []),
    "all-empty": (([], [], []), []),
    "negative": ((["a"], ["A"], [-5]), ["aA-5"]),
    "empty-strings": ((["", "b"], ["A", ""], [1, 2]), ["A1", "b2"]),
}


@pytest.mark.parametrize("name", CASES)
def test_zip_join(name):
    args, want = CASES[name]
    assert zip_join(*args) == want
