# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/set-file-permissions/.

import os
from pathlib import Path

import pytest

from set_mode import set_mode

# name: (path, mode, result)
CASES = {
    "owner-read-write-group-read": ("a.txt", "640", 0o640),
    "owner-only": ("a.txt", "600", 0o600),
    "executable": ("a.txt", "755", 0o755),
    "same-mode": ("a.txt", "644", 0o644),
    "no-permissions": ("a.txt", "000", 0),
    "leading-zero": ("a.txt", "0640", 0o640),
    "set-user-id": ("a.txt", "4755", 0o4755),
    "name-with-spaces": ("my notes.txt", "640", 0o640),
    "non-ascii-name": ("café 日本.txt", "640", 0o640),
}


def new_file(path: Path) -> Path:
    path.write_bytes(b"alpha\n")
    os.chmod(path, 0o644)
    return path


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    rel, mode, result = CASES[name]
    path = new_file(tmp_path / rel)
    assert set_mode(path, mode) == result
    assert os.stat(path).st_mode & 0o7777 == result
    assert result == 0 or path.read_bytes() == b"alpha\n"


@pytest.mark.parametrize("mode", ["648", "64", "10640", "u+x", "", "640\n"])
def test_invalid_mode(tmp_path: Path, mode: str) -> None:
    path = new_file(tmp_path / "a.txt")
    with pytest.raises(ValueError):
        set_mode(path, mode)
    assert os.stat(path).st_mode & 0o7777 == 0o644


def test_missing(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        set_mode(tmp_path / "missing.txt", "640")


def test_symbolic_link_changes_target(tmp_path: Path) -> None:
    target = new_file(tmp_path / "a.txt")
    os.symlink("a.txt", tmp_path / "link")
    assert set_mode(tmp_path / "link", "600") == 0o600
    assert os.stat(target).st_mode & 0o777 == 0o600


def test_directory(tmp_path: Path) -> None:
    (tmp_path / "dir").mkdir()
    assert set_mode(tmp_path / "dir", "700") == 0o700


def test_umask_does_not_apply(tmp_path: Path) -> None:
    path = new_file(tmp_path / "a.txt")
    old = os.umask(0o077)
    try:
        assert set_mode(path, "666") == 0o666
    finally:
        os.umask(old)
