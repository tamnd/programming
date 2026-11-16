# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

import string
from collections import Counter


def count_letters(text: str) -> list[int]:
    """Return the counts of the letters a to z in text, in the order of the
    alphabet. Upper case letters count as lower case letters."""
    found = Counter(c for c in text.lower() if c in string.ascii_lowercase)
    return [found[c] for c in string.ascii_lowercase]


if __name__ == "__main__":
    print(count_letters("Hello, World!"))
