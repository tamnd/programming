# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/round-a-number/.
import math


def round_half_away(x: float) -> int:
    """Rounds x to the nearest integer; a half goes away from zero."""
    # The built-in round() rounds a half to the even integer. Round the
    # magnitude, then set the sign. m - f is exact, so the test does not
    # round. floor(x + 0.5) is wrong for 0.49999999999999994.
    m = abs(x)
    f = math.floor(m)
    if m - f >= 0.5:
        f += 1
    return -f if x < 0 else f
