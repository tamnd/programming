# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-word-frequencies/.

"""Finds the n most frequent words of a text file."""

import re
from collections import Counter
from os import PathLike

# A word is a run of ASCII letters. The pattern runs on bytes, so the file
# needs no decoding, and each byte that is not a letter ends a word.
WORD = re.compile(rb"[a-z]+")


def word_frequencies(path: str | PathLike[str], n: int) -> list[tuple[str, int]]:
    """Returns the n most frequent words of the file at path, with their counts.

    The words are changed to lower case. The pairs are sorted by count from
    high to low, then by word. Memory grows with the number of different
    words and the length of the longest line, not with the size of the file.
    Raises FileNotFoundError if the file does not exist.
    """
    counts: Counter[bytes] = Counter()
    with open(path, "rb") as f:
        # LF is not a letter, so no word goes across two lines.
        for line in f:
            # bytes.lower changes only A to Z.
            counts.update(WORD.findall(line.lower()))
    # most_common(n) keeps equal counts in the order of first use, so sort
    # with the word as the second key.
    ranked = sorted(counts.items(), key=lambda item: (-item[1], item[0]))
    return [(word.decode("ascii"), count) for word, count in ranked[:n]]
