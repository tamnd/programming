# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/truncate-a-file/.

import errno
import os
from pathlib import Path

import pytest

from truncate_file import truncate_file

# name: (path, length, content before, content after)
CASES = {
    "shrink": ("a.txt", 5, b"alpha beta\n", b"alpha"),
    "extend": ("a.txt", 8, b"alpha", b"alpha\0\0\0"),
    "same-length": ("a.txt", 6, b"alpha\n", b"alpha\n"),
    "to-zero": ("a.txt", 0, b"alpha\n", b""),
    "empty-file": ("a.txt", 3, b"", b"\0\0\0"),
    "name-with-spaces": ("my notes.txt", 2, b"alpha\n", b"al"),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    rel, length, before, after = CASES[name]
    path = tmp_path / rel
    path.write_bytes(before)
    truncate_file(path, length)
    assert path.read_bytes() == after


def test_errors(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        truncate_file(tmp_path / "a.txt", 0)
    assert not (tmp_path / "a.txt").exists()
    (tmp_path / "dir").mkdir()
    with pytest.raises(IsADirectoryError):
        truncate_file(tmp_path / "dir", 0)
    (tmp_path / "a.txt").write_bytes(b"alpha\n")
    with pytest.raises(OSError) as e:
        truncate_file(tmp_path / "a.txt", -1)
    assert e.value.errno == errno.EINVAL
    assert (tmp_path / "a.txt").read_bytes() == b"alpha\n"


def test_open_in_append_mode(tmp_path: Path) -> None:
    path = tmp_path / "app.log"
    path.write_bytes(b"alpha beta\n")
    with open(path, "ab") as log:
        truncate_file(path, 0)
        log.write(b"new\n")
    assert path.read_bytes() == b"new\n"


def test_open_after_the_new_end(tmp_path: Path) -> None:
    path = tmp_path / "data.bin"
    path.write_bytes(b"alpha beta\n")
    with open(path, "r+b") as f:
        f.seek(0, os.SEEK_END)
        truncate_file(path, 5)
        f.write(b"X")
    assert path.read_bytes() == b"alpha" + b"\0" * 6 + b"X"
