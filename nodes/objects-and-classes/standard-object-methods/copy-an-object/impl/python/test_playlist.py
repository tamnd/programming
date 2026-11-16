# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/copy-an-object/.

import copy

import pytest

from playlist import Playlist, copy_and_change

CASES = {
    "middle": ((["Intro", "Song", "Outro"], 1, "Hit"),
               [["Intro", "Hit", "Outro"], ["Intro", "Hit", "Outro"], ["Intro", "Song", "Outro"]]),
    "two": ((["A", "B"], 0, "C"), [["C", "B"], ["C", "B"], ["A", "B"]]),
    "single": ((["Solo"], 0, "Duet"), [["Duet"], ["Duet"], ["Solo"]]),
    "first": ((["a", "b", "c"], 0, "z"), [["z", "b", "c"], ["z", "b", "c"], ["a", "b", "c"]]),
    "last": ((["a", "b", "c"], 2, "z"), [["a", "b", "z"], ["a", "b", "z"], ["a", "b", "c"]]),
    "same-title": ((["a", "b"], 1, "b"), [["a", "b"], ["a", "b"], ["a", "b"]]),
    "duplicates": ((["x", "x", "x"], 1, "y"), [["x", "y", "x"], ["x", "y", "x"], ["x", "x", "x"]]),
}


@pytest.mark.parametrize("name", CASES)
def test_copy_and_change(name):
    args, want = CASES[name]
    assert copy_and_change(*args) == want


def test_copies_are_new_objects():
    original = Playlist("mix", ["a"])
    shallow, deep = copy.copy(original), copy.deepcopy(original)
    assert shallow is not original and deep is not original
    assert shallow.songs is original.songs and deep.songs is not original.songs
    shallow.name = "other"
    assert original.name == "mix"


def test_bad_index():
    with pytest.raises(IndexError):
        copy_and_change(["a"], 1, "b")
