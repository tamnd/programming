# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-function/.


def clamp(value: int, low: int, high: int) -> int:
    """Return value limited to the range from low to high."""
    if value < low:
        return low
    if value > high:
        return high
    return value
