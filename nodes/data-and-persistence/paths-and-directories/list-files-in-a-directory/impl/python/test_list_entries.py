# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/list-files-in-a-directory/.

import os
from pathlib import Path

import pytest

from list_entries import list_entries

TREE = ["docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"]

CASES = {
    "flat": (["b.txt", "a.txt", "c.txt"], False, ["a.txt", "b.txt", "c.txt"]),
    "nested": (TREE, True, TREE),
    "nested-shallow": (TREE, False, ["docs/", "readme.md"]),
    "empty": ([], True, []),
    "empty-subdir": (["empty/"], True, ["empty/"]),
    "hidden": ([".env", ".git/", ".git/config", "main.go"], True, [".env", ".git/", ".git/config", "main.go"]),
    "case-order": (["a.txt", "B.txt", "c.txt"], False, ["B.txt", "a.txt", "c.txt"]),
    "dot-before-slash": (["a/", "a/x", "a.txt", "a-b"], True, ["a-b", "a.txt", "a/", "a/x"]),
    "symlink-to-dir": (["real/", "real/f", "link -> real"], True, ["link", "real/", "real/f"]),
    "symlink-to-file": (["f", "g -> f"], True, ["f", "g"]),
    "broken-symlink": (["dangling -> nowhere"], True, ["dangling"]),
    "symlink-loop": (["d/", "d/up -> .."], True, ["d/", "d/up"]),
    "spaces": (["my file.txt", "two  spaces/", "two  spaces/x"], True, ["my file.txt", "two  spaces/", "two  spaces/x"]),
    "non-ascii": (["été.txt", "zebra.txt", "apple.txt"], False, ["apple.txt", "zebra.txt", "été.txt"]),
}


def make_tree(root: Path, tree: list[str]) -> None:
    """Make each entry of the tree under root, in order."""
    for entry in tree:
        if " -> " in entry:
            name, target = entry.split(" -> ")
            os.symlink(target, root / name)
        elif entry.endswith("/"):
            (root / entry).mkdir()
        else:
            (root / entry).touch()


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    tree, recursive, want = CASES[name]
    make_tree(tmp_path, tree)
    assert list_entries(tmp_path, recursive) == want


def test_missing(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        list_entries(tmp_path / "missing", True)


def test_not_a_directory(tmp_path: Path) -> None:
    make_tree(tmp_path, ["notes.txt"])
    with pytest.raises(NotADirectoryError):
        list_entries(tmp_path / "notes.txt", True)
