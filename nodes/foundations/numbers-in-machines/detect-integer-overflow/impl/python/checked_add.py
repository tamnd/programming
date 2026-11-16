# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-integer-overflow/.
INT64_MIN = -(2**63)
INT64_MAX = 2**63 - 1


def checked_add(a: int, b: int) -> int | None:
    """Returns a + b, or None when the sum does not fit in 64 signed bits."""
    # A Python int has no fixed width, so the sum is exact. Compare it with the
    # range of the 64-bit type after the addition.
    s = a + b
    return s if INT64_MIN <= s <= INT64_MAX else None
