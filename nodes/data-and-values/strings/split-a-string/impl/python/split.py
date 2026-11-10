# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/split-a-string/.

import re


def split(s: str, sep: str, limit: int = 0) -> list[str]:
    """Split s on the literal separator sep and keep the empty fields.

    With a limit n > 0 the result has at most n fields, and the last field
    holds the rest of s. A limit of 0 means no limit.
    """
    return s.split(sep, limit - 1 if limit > 0 else -1)


def split_whitespace(s: str) -> list[str]:
    """Split s on runs of white space. The result has no empty fields."""
    return s.split()


def split_regex(s: str, pattern: str) -> list[str]:
    """Split s on each match of the regular expression pattern."""
    return re.split(pattern, s)
