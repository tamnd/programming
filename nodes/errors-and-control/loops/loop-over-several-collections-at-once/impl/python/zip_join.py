# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

def zip_join(lower: list[str], upper: list[str], digits: list[int]) -> list[str]:
    """Join the items at each index, up to the end of the shortest list."""
    out = []
    # zip stops at the end of the shortest list.
    for a, b, d in zip(lower, upper, digits):
        out.append(f"{a}{b}{d}")
    return out
