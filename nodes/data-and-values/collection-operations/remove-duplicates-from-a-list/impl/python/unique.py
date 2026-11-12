# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

from collections.abc import Callable, Hashable, Iterable
from typing import NamedTuple


class User(NamedTuple):
    name: str
    email: str


def unique[T: Hashable](items: Iterable[T]) -> list[T]:
    """Return the items without later copies. The first copies keep their order."""
    # A dict keeps its keys in insertion order. A set does not.
    return list(dict.fromkeys(items))


def unique_by[T, K: Hashable](items: Iterable[T], key: Callable[[T], K]) -> list[T]:
    """Return the first item for each key, in input order."""
    seen: set[K] = set()
    out: list[T] = []
    for item in items:
        k = key(item)
        if k not in seen:
            seen.add(k)
            out.append(item)
    return out


def unique_by_email(users: Iterable[User]) -> list[User]:
    """Return the first user for each email. Case does not matter in the emails."""
    return unique_by(users, lambda u: u.email.lower())
