# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

import pytest

from gcd import gcd_and_lcm, gcd_and_lcm_euclid

CASES = [
    ("common-factor", 12, 18, (6, 36)),
    ("coprime", 17, 5, (1, 85)),
    ("euclid-example", 48, 180, (12, 720)),
    ("zero-and-number", 0, 7, (7, 0)),
    ("both-zero", 0, 0, (0, 0)),
    ("negative", -12, 18, (6, 36)),
    ("both-negative", -4, -6, (2, 12)),
    ("equal", 9, 9, (9, 9)),
    ("one", 1, 1000000007, (1, 1000000007)),
    ("multiple", 5, 25, (5, 25)),
    ("fibonacci", 832040, 514229, (1, 427859097160)),
    ("big-lcm", 3000000000000000000, 6000000000000000000, (3000000000000000000, 6000000000000000000)),
    ("largest", 9223372036854775807, -9223372036854775807, (9223372036854775807, 9223372036854775807)),
]


@pytest.mark.parametrize(("name", "a", "b", "want"), CASES, ids=[c[0] for c in CASES])
def test_gcd_and_lcm(name, a, b, want):
    assert gcd_and_lcm(a, b) == want
    assert gcd_and_lcm_euclid(a, b) == want
    assert gcd_and_lcm(b, a) == want
