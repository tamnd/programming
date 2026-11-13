# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/copy-a-file/.

import os
import shutil
import stat
from pathlib import Path

import pytest

from copy_file import copy_file

# name: (src, dst, content of src, content of dst or None)
CASES = {
    "copy": ("a.txt", "b.txt", "alpha\n", None),
    "replace-existing": ("a.txt", "b.txt", "alpha\n", "beta, a longer text\n"),
    "empty-file": ("a.txt", "b.txt", "", "beta\n"),
    "other-directory": ("a.txt", "sub/a.txt", "alpha\n", None),
    "name-with-spaces": ("my notes.txt", "copy of my notes.txt", "alpha\n", None),
    "non-ascii-name": ("a.txt", "café 日本.txt", "alpha\n", None),
    "binary-data": ("a.bin", "b.bin", "\0\r\n\xff\0", None),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    src_rel, dst_rel, src_content, dst_content = CASES[name]
    src, dst = tmp_path / src_rel, tmp_path / dst_rel
    dst.parent.mkdir(exist_ok=True)
    if dst_content is not None:
        dst.write_bytes(dst_content.encode())
    src.write_bytes(src_content.encode())
    copy_file(src, dst)
    assert dst.read_bytes() == src_content.encode()
    assert src.read_bytes() == src_content.encode()


def test_missing(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        copy_file(tmp_path / "a.txt", tmp_path / "b.txt")
    (tmp_path / "a.txt").write_text("alpha\n")
    with pytest.raises(FileNotFoundError):
        copy_file(tmp_path / "a.txt", tmp_path / "no-such-dir" / "b.txt")


def test_directories(tmp_path: Path) -> None:
    (tmp_path / "dir").mkdir()
    (tmp_path / "a.txt").write_text("alpha\n")
    with pytest.raises(IsADirectoryError):
        copy_file(tmp_path / "dir", tmp_path / "b.txt")
    with pytest.raises(IsADirectoryError):
        copy_file(tmp_path / "a.txt", tmp_path / "dir")
    assert os.listdir(tmp_path / "dir") == []
    assert not (tmp_path / "b.txt").exists()


def test_same_file(tmp_path: Path) -> None:
    src = tmp_path / "a.txt"
    src.write_text("alpha\n")
    os.link(src, tmp_path / "b.txt")
    for dst in (src, tmp_path / "b.txt"):
        with pytest.raises(shutil.SameFileError):
            copy_file(src, dst)
        assert src.read_text() == "alpha\n"


def test_mode(tmp_path: Path) -> None:
    src, dst = tmp_path / "a.sh", tmp_path / "b.sh"
    src.write_text("echo alpha\n")
    os.chmod(src, 0o750)
    copy_file(src, dst)
    assert stat.S_IMODE(dst.stat().st_mode) == 0o750
    other = tmp_path / "c.sh"
    other.write_text("beta\n")
    os.chmod(other, 0o640)
    copy_file(src, other)
    assert stat.S_IMODE(other.stat().st_mode) == 0o750


def test_large_file(tmp_path: Path) -> None:
    data = bytes(i * 7 % 251 for i in range(5_000_003))
    src, dst = tmp_path / "big.bin", tmp_path / "copy.bin"
    src.write_bytes(data)
    copy_file(src, dst)
    assert dst.read_bytes() == data


def test_link_at_src(tmp_path: Path) -> None:
    (tmp_path / "target.txt").write_text("alpha\n")
    os.symlink("target.txt", tmp_path / "link.txt")
    copy_file(tmp_path / "link.txt", tmp_path / "b.txt")
    assert not (tmp_path / "b.txt").is_symlink()
    assert (tmp_path / "b.txt").read_text() == "alpha\n"
