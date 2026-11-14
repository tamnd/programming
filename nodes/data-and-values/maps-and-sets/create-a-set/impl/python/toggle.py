# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-set/.


def toggle(words: list[str]) -> list[str]:
    """Add each word that is not in the set, remove each word that is in it."""
    # set() makes an empty set. {} would make an empty dict.
    seen = set()
    for word in words:
        if word in seen:
            seen.remove(word)
        else:
            seen.add(word)
    return sorted(seen)
