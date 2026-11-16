# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-anagrams/.

from collections import Counter


def letter_counts(text: str) -> Counter[str]:
    """Count the ASCII letters of text, with upper case as lower case."""
    return Counter(c.lower() for c in text if c.isascii() and c.isalpha())


def are_anagrams(a: str, b: str) -> bool:
    """Return True if a and b hold each letter the same number of times.
    Case, spaces, punctuation and digits do not count."""
    return letter_counts(a) == letter_counts(b)


if __name__ == "__main__":
    print(are_anagrams("Dormitory", "dirty room"))
