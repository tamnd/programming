# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-variable/.


def total(values: list[int]) -> int:
    """Return the sum of values. The function declares the variable result
    with a type annotation, initializes it to 0 and reassigns it once for
    each value."""
    result: int = 0
    for value in values:
        result = result + value
    return result


if __name__ == "__main__":
    print(total([1, 2, 3]))
