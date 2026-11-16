# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-division-by-zero/.
INT64_MIN = -(2**63)
INT64_MAX = 2**63 - 1


def safe_div(a: int, b: int) -> int | None:
    """Returns a / b rounded toward zero, or None for a zero divisor or a
    quotient that does not fit in 64 signed bits."""
    if b == 0:
        return None  # a // 0 raises ZeroDivisionError
    # // rounds toward minus infinity, so divide the magnitudes and set the
    # sign after, to round toward zero.
    q = abs(a) // abs(b)
    if (a < 0) != (b < 0):
        q = -q
    return q if INT64_MIN <= q <= INT64_MAX else None
