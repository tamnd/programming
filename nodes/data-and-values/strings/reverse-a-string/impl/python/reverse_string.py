# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-string/.


def reverse_code_points(s: str) -> str:
    """Return the code points of s in the opposite order.

    A Python str is a sequence of code points, so a slice with step -1
    reverses code points. It does not keep a combining mark on its base
    letter, and it does not keep the parts of an emoji sequence together.
    """
    return s[::-1]
