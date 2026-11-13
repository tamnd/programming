# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-with-a-custom-key/.


def sort_by_length(words: list[str]) -> list[str]:
    """Return the words in order of the length, then of the word."""
    # A tuple key compares the length first, then the word.
    return sorted(words, key=lambda w: (len(w), w))
