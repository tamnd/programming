# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/copy-an-object/.

"""A shallow copy and a deep copy of an object with a list field."""

import copy


class Playlist:
    def __init__(self, name: str, songs: list[str]) -> None:
        self.name = name
        self.songs = songs


def copy_and_change(songs: list[str], index: int, title: str) -> list[list[str]]:
    if not 0 <= index < len(songs):
        raise IndexError(f"no song at index {index}")
    original = Playlist("mix", list(songs))
    shallow = copy.copy(original)  # a new Playlist; its songs field is the same list
    deep = copy.deepcopy(original)  # a new Playlist with a new list
    original.songs[index] = title
    return [list(original.songs), list(shallow.songs), list(deep.songs)]
