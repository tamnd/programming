# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

from functools import total_ordering


@total_ordering
class Version:
    """A version MAJOR.MINOR.PATCH. The natural order compares the numbers, most important first."""

    __slots__ = ("_parts",)

    def __init__(self, major, minor, patch):
        self._parts = (major, minor, patch)

    @classmethod
    def parse(cls, text):
        major, minor, patch = (int(p) for p in text.split("."))
        return cls(major, minor, patch)

    def __eq__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._parts == other._parts

    def __lt__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._parts < other._parts  # tuples compare item by item

    def __hash__(self):
        return hash(self._parts)

    def __str__(self):
        return ".".join(str(p) for p in self._parts)

    def __repr__(self):
        return f"Version({self._parts[0]}, {self._parts[1]}, {self._parts[2]})"


def sort_versions(versions):
    return [str(v) for v in sorted(Version.parse(s) for s in versions)]
