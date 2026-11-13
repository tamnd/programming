# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

import os
import time
from pathlib import Path

import pytest

from file_mtime import file_mtime

# name: (path, mtime_ns, result)
CASES = {
    "whole-second": ("a.txt", 1709296496_000000000, 1709296496),
    "sub-second": ("a.txt", 1709296496_500000000, 1709296496),
    "epoch": ("a.txt", 0, 0),
    "before-1970": ("a.txt", -86400_000000000, -86400),
    "before-1970-sub-second": ("a.txt", -1_500000000, -2),
    "after-2038": ("a.txt", 4102444800_000000000, 4102444800),
    "directory": ("dir", 1709296496_000000000, 1709296496),
    "name-with-spaces": ("my notes.txt", 1709296496_000000000, 1709296496),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    rel, ns, want = CASES[name]
    path = tmp_path / rel
    if rel == "dir":
        path.mkdir()
    else:
        path.write_text("alpha\n")
    os.utime(path, ns=(ns, ns))
    assert file_mtime(path) == want


def test_errors(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        file_mtime(tmp_path / "a.txt")


def test_symbolic_link(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_text("alpha\n")
    os.utime(tmp_path / "a.txt", ns=(0, 1709296496_000000000))
    os.symlink("a.txt", tmp_path / "link")
    assert file_mtime(tmp_path / "link") == 1709296496


def test_write_sets_the_present_time(tmp_path: Path) -> None:
    path = tmp_path / "a.txt"
    path.write_text("alpha\n")
    os.utime(path, ns=(0, 0))
    before = int(time.time()) - 1
    with open(path, "a") as f:
        f.write("beta\n")
    assert file_mtime(path) >= before
