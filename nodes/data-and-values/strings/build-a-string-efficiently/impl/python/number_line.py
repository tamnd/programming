# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/build-a-string-efficiently/.


def number_line(n: int) -> str:
    """Return the numbers 1 to n, separated by commas."""
    # A str is immutable, so the code collects the parts in a list and
    # joins them once at the end.
    parts = []
    for i in range(1, n + 1):
        parts.append(str(i))
    return ",".join(parts)
