# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-word-frequencies/.

import re
from collections import Counter

# A word is a run of ASCII letters, ASCII digits and characters that are not ASCII.
WORD = re.compile(r"[A-Za-z0-9\u0080-\U0010FFFF]+")


def top_words(text: str, n: int) -> list[tuple[str, int]]:
    """Return the n most frequent words of text with their counts.

    The order is by count from high to low, then by word in code point order.
    """
    counts = Counter(word.lower() for word in WORD.findall(text))
    # most_common orders equal counts by first occurrence, so sort with the
    # word as the second key.
    return sorted(counts.items(), key=lambda item: (-item[1], item[0]))[:n]
