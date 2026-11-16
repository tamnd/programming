# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.


def fixed(x: float, digits: int) -> str:
    """Return x with exactly digits digits after the decimal point. The
    exact binary value of x is rounded."""
    return f"{x:.{digits}f}"


if __name__ == "__main__":
    print(fixed(3.14159, 2))
    print(fixed(2.675, 2))
