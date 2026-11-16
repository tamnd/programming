# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

import string


def caesar(text: str, shift: int) -> str:
    """Move each letter A to Z and a to z shift places along the alphabet,
    from z back to a. Other characters do not change. A negative shift
    decodes, and caesar(text, 13) is ROT13."""
    k = shift % 26
    lower, upper = string.ascii_lowercase, string.ascii_uppercase
    table = str.maketrans(lower + upper, lower[k:] + lower[:k] + upper[k:] + upper[:k])
    return text.translate(table)


if __name__ == "__main__":
    print(caesar("Hello, World!", 13))
