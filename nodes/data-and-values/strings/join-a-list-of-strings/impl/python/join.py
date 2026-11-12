# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/join-a-list-of-strings/.


def join(items: list[str], sep: str) -> str:
    """Return the items with sep between each two."""
    # join is a method of the separator, and it takes any iterable of str.
    return sep.join(items)


def join_numbers(items: list[int], sep: str) -> str:
    """Return the decimal form of each item, with sep between each two."""
    # join raises TypeError for an item that is not a str, so convert first.
    return sep.join(map(str, items))
