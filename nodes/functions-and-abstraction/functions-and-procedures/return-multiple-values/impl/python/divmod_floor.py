# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/return-multiple-values/.


def div_mod(a: int, b: int) -> tuple[int, int]:
    """Return the quotient and the remainder of the floor division of a by b."""
    return a // b, a % b
