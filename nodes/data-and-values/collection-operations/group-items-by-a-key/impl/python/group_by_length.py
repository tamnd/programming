# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/group-items-by-a-key/.

from collections import defaultdict


def group_by_length(words: list[str]) -> list[tuple[int, list[str]]]:
    """Return one (length, words) pair for each length, in ascending order of the length."""
    groups: defaultdict[int, list[str]] = defaultdict(list)
    for word in words:
        groups[len(word)].append(word)
    return sorted(groups.items())
