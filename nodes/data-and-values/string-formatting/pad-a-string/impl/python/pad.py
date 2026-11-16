# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pad-a-string/.


def pad(s: str, width: int, fill: str) -> tuple[str, str]:
    """Return s padded with fill to width, once on the left and once on the
    right. If s has width characters or more, both results are s."""
    return s.rjust(width, fill), s.ljust(width, fill)


if __name__ == "__main__":
    print(pad("7", 3, "0"))
    print(pad("abc", 6, "*"))
