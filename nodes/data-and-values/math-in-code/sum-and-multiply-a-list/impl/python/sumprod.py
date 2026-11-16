# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

import math


def sum_and_product(items: list[int]) -> tuple[int, int]:
    """Return the sum and the product of the items."""
    return sum(items), math.prod(items)


def sum_and_product_loop(items: list[int]) -> tuple[int, int]:
    """Return the same pair with a loop."""
    total, product = 0, 1
    for x in items:
        total += x
        product *= x
    return total, product
