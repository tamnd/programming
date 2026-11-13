# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-size-of-a-file/.

import os
from pathlib import Path

import pytest

from file_size import file_size

# name: (path, content, size)
CASES = {
    "text": ("a.txt", "alpha\n".encode(), 6),
    "non-ascii-text": ("a.txt", "café\n".encode(), 6),
    "empty-file": ("a.txt", b"", 0),
    "binary-data": ("a.bin", b"\x00\r\n\xc3\xbf\x00", 6),
    "crlf-line-ends": ("a.txt", b"a\r\nb\r\n", 6),
    "name-with-spaces": ("my notes.txt", b"alpha\n", 6),
    "non-ascii-name": ("café 日本.txt", b"alpha\n", 6),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    rel, data, size = CASES[name]
    path = tmp_path / rel
    path.write_bytes(data)
    assert file_size(path) == size
    assert path.read_bytes() == data


def test_errors(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        file_size(tmp_path / "a.txt")
    (tmp_path / "dir").mkdir()
    with pytest.raises(IsADirectoryError):
        file_size(tmp_path / "dir")


def test_symbolic_link(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_bytes(b"alpha beta\n")
    os.symlink("a.txt", tmp_path / "link")
    assert file_size(tmp_path / "link") == 11


def test_sparse_file_of_5_gib(tmp_path: Path) -> None:
    path = tmp_path / "sparse.bin"
    path.write_bytes(b"")
    os.truncate(path, 5 * 1024**3)
    assert file_size(path) == 5368709120


def test_open_file_after_flush(tmp_path: Path) -> None:
    path = tmp_path / "app.log"
    with open(path, "wb") as f:
        f.write(b"alpha\n")
        f.flush()
        assert file_size(path) == 6
        f.write(b"beta\n")
        f.flush()
        assert file_size(path) == 11
