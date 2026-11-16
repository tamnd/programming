# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-big-integers/.


def big_power(base: int, exp: int) -> str:
    """Returns base to the power exp as decimal text. A Python int has no
    fixed width, so the result is exact."""
    return str(base**exp)
