# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-record-type/.

from dataclasses import dataclass, replace


# frozen=True makes the fields read-only. The decorator also writes
# __init__, __repr__ and an __eq__ that compares the fields in order.
@dataclass(frozen=True)
class Person:
    name: str
    age: int


def birthday(people: list[Person], name: str) -> list[Person]:
    """Return a new list where each person called name is one year older."""
    return [replace(p, age=p.age + 1) if p.name == name else p for p in people]
