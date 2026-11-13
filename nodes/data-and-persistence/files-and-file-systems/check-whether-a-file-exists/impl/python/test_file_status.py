# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-file-exists/.

import os
from pathlib import Path

import pytest

from file_status import file_status


@pytest.fixture
def tree(tmp_path: Path) -> Path:
    (tmp_path / "a.txt").write_text("alpha\n")
    (tmp_path / "empty.txt").write_bytes(b"")
    (tmp_path / "dir").mkdir()
    return tmp_path


CASES = {
    "a.txt": "file",
    "empty.txt": "file",
    "dir": "directory",
    "dir/": "directory",
    "missing.txt": "missing",
    "no-such-dir/a.txt": "missing",
    "a.txt/b.txt": "missing",
}


@pytest.mark.parametrize("name", CASES)
def test_status(tree: Path, name: str) -> None:
    assert file_status(os.path.join(tree, name)) == CASES[name]


def test_links(tree: Path) -> None:
    (tree / "to-file").symlink_to("a.txt")
    (tree / "to-dir").symlink_to("dir")
    (tree / "to-nothing").symlink_to("missing.txt")
    assert file_status(tree / "to-file") == "file"
    assert file_status(tree / "to-dir") == "directory"
    assert file_status(tree / "to-nothing") == "broken-link"


@pytest.mark.skipif(not os.path.exists("/dev/null"), reason="no /dev/null")
def test_device() -> None:
    assert file_status("/dev/null") == "other"


def test_named_pipe(tree: Path) -> None:
    os.mkfifo(tree / "pipe")
    assert file_status(tree / "pipe") == "other"


@pytest.mark.skipif(os.geteuid() == 0, reason="root can search each directory")
def test_permission_error(tree: Path) -> None:
    locked = tree / "locked"
    locked.mkdir()
    (locked / "a.txt").write_text("alpha\n")
    locked.chmod(0o000)
    try:
        with pytest.raises(PermissionError):
            file_status(locked / "a.txt")
    finally:
        locked.chmod(0o700)
