# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-integer-arithmetic/.


def arith(a: int, b: int, n: int) -> list[int]:
    """Returns the sum, the difference, the product, the quotient rounded
    toward zero, the remainder with the sign of a, and a to the power n."""
    # Python // rounds toward minus infinity and % takes the sign of b. The
    # contract asks for truncated division, so divide the magnitudes and set
    # the sign after.
    q = abs(a) // abs(b)
    if (a < 0) != (b < 0):
        q = -q
    r = a - b * q
    return [a + b, a - b, a * b, q, r, a**n]
