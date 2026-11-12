# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-a-substring/.


def replace(s: str, old: str, new: str) -> str:
    """Return s with each match of old replaced by new."""
    return s.replace(old, new)


def replace_first(s: str, old: str, new: str) -> str:
    """Return s with the first match of old replaced by new."""
    # The third argument is the largest number of matches to replace.
    return s.replace(old, new, 1)
