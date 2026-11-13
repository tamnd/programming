# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/delete-a-file/.

import os
from pathlib import Path

import pytest

from delete_file import delete_file

# name: (path, content before the call or None, result)
CASES = {
    "delete": ("a.txt", "alpha\n", True),
    "missing": ("a.txt", None, False),
    "empty-file": ("a.txt", "", True),
    "missing-parent": ("no-such-dir/a.txt", None, False),
    "name-with-spaces": ("my notes.txt", "alpha\n", True),
    "non-ascii-name": ("café 日本.txt", "alpha\n", True),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    rel, content, want = CASES[name]
    path = tmp_path / rel
    if content is not None:
        path.write_text(content, encoding="utf-8")
    assert delete_file(path) is want
    assert not os.path.lexists(path)


def test_through_a_file(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_text("alpha\n")
    assert delete_file(tmp_path / "a.txt" / "b.txt") is False
    assert (tmp_path / "a.txt").exists()


def test_directory(tmp_path: Path) -> None:
    (tmp_path / "dir").mkdir()
    with pytest.raises(OSError):
        delete_file(tmp_path / "dir")
    assert (tmp_path / "dir").is_dir()


def test_link_to_a_file(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_text("alpha\n")
    os.symlink("a.txt", tmp_path / "link")
    assert delete_file(tmp_path / "link") is True
    assert not os.path.lexists(tmp_path / "link")
    assert (tmp_path / "a.txt").read_text() == "alpha\n"


def test_link_to_a_directory(tmp_path: Path) -> None:
    (tmp_path / "dir").mkdir()
    os.symlink("dir", tmp_path / "link")
    assert delete_file(tmp_path / "link") is True
    assert not os.path.lexists(tmp_path / "link")
    assert (tmp_path / "dir").is_dir()


def test_broken_link(tmp_path: Path) -> None:
    os.symlink("missing.txt", tmp_path / "link")
    assert delete_file(tmp_path / "link") is True
    assert not os.path.lexists(tmp_path / "link")


def test_read_only_file(tmp_path: Path) -> None:
    path = tmp_path / "a.txt"
    path.write_text("alpha\n")
    path.chmod(0o444)
    assert delete_file(path) is True
    assert not path.exists()


@pytest.mark.skipif(os.geteuid() == 0, reason="root can write to each directory")
def test_directory_with_no_write_permission(tmp_path: Path) -> None:
    locked = tmp_path / "locked"
    locked.mkdir()
    path = locked / "a.txt"
    path.write_text("alpha\n")
    locked.chmod(0o500)
    try:
        with pytest.raises(PermissionError):
            delete_file(path)
        assert path.exists()
    finally:
        locked.chmod(0o700)


def test_open_file(tmp_path: Path) -> None:
    path = tmp_path / "a.txt"
    path.write_text("alpha\n")
    with open(path) as f:
        assert delete_file(path) is True
        assert not path.exists()
        assert f.read() == "alpha\n"  # the data stays until the close
