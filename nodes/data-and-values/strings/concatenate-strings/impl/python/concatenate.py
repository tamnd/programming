# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-strings/.


def concat(a: str, b: str) -> str:
    """Return the characters of a followed by the characters of b."""
    return a + b


def concat_number(s: str, n: int) -> str:
    """Return s followed by the decimal form of n."""
    # str + int raises TypeError, so the f-string converts n.
    return f"{s}{n}"
