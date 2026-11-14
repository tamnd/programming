# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.


def lookup(table: dict[str, int], keys: list[str], fallback: int) -> list[int]:
    """Return the value of each key in table, or fallback for a key not in table."""
    # get does not add the key, and it returns a stored 0 as 0.
    return [table.get(key, fallback) for key in keys]
