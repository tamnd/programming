# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-symbolic-link/.

import os
from pathlib import Path

import pytest

from make_symlink import make_symlink

# name: (target, link)
CASES = {
    "file-target": ("a.txt", "link"),
    "directory-target": ("sub", "link"),
    "link-in-subdirectory": ("b.txt", "sub/link"),
    "dangling-target": ("missing.txt", "link"),
    "relative-target": ("a.txt", "sub/link"),
    "parent-target": ("../a.txt", "sub/link"),
    "name-with-spaces": ("my notes.txt", "my link"),
    "non-ascii-name": ("café 日本.txt", "lien 日本"),
}


@pytest.fixture
def tree(tmp_path: Path) -> Path:
    (tmp_path / "a.txt").write_bytes(b"alpha\n")
    (tmp_path / "sub").mkdir()
    (tmp_path / "sub" / "b.txt").write_bytes(b"beta\n")
    return tmp_path


@pytest.mark.parametrize("name", CASES)
def test_cases(tree: Path, name: str) -> None:
    target, rel = CASES[name]
    link = tree / rel
    assert make_symlink(target, link) == target
    assert link.is_symlink()


@pytest.mark.parametrize("target,rel", [("b.txt", "a.txt"), ("a.txt", "sub")])
def test_exists(tree: Path, target: str, rel: str) -> None:
    with pytest.raises(FileExistsError):
        make_symlink(target, tree / rel)
    assert not (tree / rel).is_symlink()


def test_dangling_link_exists(tree: Path) -> None:
    os.symlink("missing.txt", tree / "link")
    assert not os.path.exists(tree / "link")
    with pytest.raises(FileExistsError):
        make_symlink("a.txt", tree / "link")
    assert os.readlink(tree / "link") == "missing.txt"


def test_missing_directory(tree: Path) -> None:
    with pytest.raises(FileNotFoundError):
        make_symlink("a.txt", tree / "nodir" / "link")


def test_link_reads_target(tree: Path) -> None:
    make_symlink("a.txt", tree / "link")
    assert (tree / "link").read_bytes() == b"alpha\n"
    make_symlink("../a.txt", tree / "sub" / "up")
    assert (tree / "sub" / "up").read_bytes() == b"alpha\n"
    make_symlink("a.txt", tree / "sub" / "here")
    assert not (tree / "sub" / "here").exists()


def test_absolute_target(tree: Path) -> None:
    target = str(tree / "a.txt")
    assert make_symlink(target, tree / "link") == target
