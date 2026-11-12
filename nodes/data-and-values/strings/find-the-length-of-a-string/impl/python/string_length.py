# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-length-of-a-string/.


def lengths(s: str) -> list[int]:
    """Return the length of s in UTF-8 bytes, in UTF-16 code units and in code points."""
    utf8 = len(s.encode("utf-8"))
    # Each UTF-16 code unit is 2 bytes. The -le codec writes no byte order mark.
    utf16 = len(s.encode("utf-16-le")) // 2
    # A str is a sequence of code points, so len counts code points.
    return [utf8, utf16, len(s)]
