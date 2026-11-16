# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-number-with-separators/.


def group_thousands(n: int) -> str:
    """Return n in decimal with a comma between each group of 3 digits."""
    return f"{n:,}"


if __name__ == "__main__":
    print(group_thousands(1234567))
    print(group_thousands(-1000))
