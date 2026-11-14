# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-map/.


def build_map(pairs: list[tuple[str, int]], remove: list[str]) -> list[tuple[str, int]]:
    """Put the pairs in a dict, delete the keys in remove, and return the entries sorted by key."""
    table: dict[str, int] = {}
    for key, value in pairs:
        # An assignment adds the key, or replaces the value of a key that is there.
        table[key] = value
    for key in remove:
        # pop with a default does not raise KeyError for a key that is not there.
        table.pop(key, None)
    return sorted(table.items())
