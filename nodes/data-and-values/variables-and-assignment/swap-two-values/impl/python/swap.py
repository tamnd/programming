# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/swap-two-values/.


def swap_items(xs: list[int], i: int, j: int) -> list[int]:
    """Return a copy of xs with the items at i and j swapped. Python
    computes the right side, a tuple of two values, before it assigns
    them to the targets on the left."""
    out = list(xs)
    out[i], out[j] = out[j], out[i]
    return out


if __name__ == "__main__":
    a, b = 1, 2
    a, b = b, a
    print(a, b, swap_items([1, 2, 3], 0, 2))
