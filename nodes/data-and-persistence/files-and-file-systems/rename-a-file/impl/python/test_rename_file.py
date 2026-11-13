# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rename-a-file/.

import errno
import os
import threading
from pathlib import Path

import pytest

from rename_file import rename_file

# name: (src, dst, content of src, content of dst or None, content of dst after)
CASES = {
    "rename": ("a.txt", "b.txt", "alpha\n", None, "alpha\n"),
    "replace-existing": ("a.txt", "b.txt", "alpha\n", "beta\n", "alpha\n"),
    "same-path": ("a.txt", "a.txt", "alpha\n", "alpha\n", "alpha\n"),
    "empty-file": ("a.txt", "b.txt", "", "beta\n", ""),
    "other-directory": ("a.txt", "sub/a.txt", "alpha\n", None, "alpha\n"),
    "name-with-spaces": ("my notes.txt", "old notes.txt", "alpha\n", None, "alpha\n"),
    "non-ascii-name": ("a.txt", "café 日本.txt", "alpha\n", None, "alpha\n"),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    src_rel, dst_rel, src_content, dst_content, want = CASES[name]
    src, dst = tmp_path / src_rel, tmp_path / dst_rel
    dst.parent.mkdir(exist_ok=True)
    if dst_content is not None:
        dst.write_text(dst_content, encoding="utf-8")
    src.write_text(src_content, encoding="utf-8")
    rename_file(src, dst)
    assert dst.read_text(encoding="utf-8") == want
    assert src == dst or not os.path.lexists(src)


def test_missing_src(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        rename_file(tmp_path / "a.txt", tmp_path / "b.txt")
    assert not os.path.lexists(tmp_path / "b.txt")


def test_missing_dst_dir(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_text("alpha\n")
    with pytest.raises(FileNotFoundError):
        rename_file(tmp_path / "a.txt", tmp_path / "no-such-dir" / "b.txt")
    assert (tmp_path / "a.txt").read_text() == "alpha\n"


def test_dst_is_a_directory(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_text("alpha\n")
    (tmp_path / "dir").mkdir()
    with pytest.raises(OSError):
        rename_file(tmp_path / "a.txt", tmp_path / "dir")
    assert (tmp_path / "dir").is_dir()
    assert (tmp_path / "a.txt").read_text() == "alpha\n"


def test_link(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_text("alpha\n")
    os.symlink("a.txt", tmp_path / "link")
    rename_file(tmp_path / "link", tmp_path / "new-link")
    assert os.readlink(tmp_path / "new-link") == "a.txt"
    assert not os.path.lexists(tmp_path / "link")
    assert (tmp_path / "a.txt").read_text() == "alpha\n"


def test_open_dst_keeps_old_data(tmp_path: Path) -> None:
    (tmp_path / "a.txt").write_text("alpha\n")
    (tmp_path / "b.txt").write_text("beta\n")
    with open(tmp_path / "b.txt") as old:
        rename_file(tmp_path / "a.txt", tmp_path / "b.txt")
        assert old.read() == "beta\n"
    assert (tmp_path / "b.txt").read_text() == "alpha\n"


def test_readers_see_a_whole_file(tmp_path: Path) -> None:
    dst = tmp_path / "data.txt"
    texts = ["a" * 4096 + "\n", "b" * 4096 + "\n"]
    dst.write_text(texts[0])
    stop = threading.Event()
    seen = set()

    def read() -> None:
        while not stop.is_set():
            seen.add(dst.read_text())

    reader = threading.Thread(target=read)
    reader.start()
    try:
        for i in range(200):
            tmp = tmp_path / "data.txt.tmp"
            tmp.write_text(texts[i % 2])
            rename_file(tmp, dst)
    finally:
        stop.set()
        reader.join()
    assert seen <= set(texts)


def test_cross_device(tmp_path: Path) -> None:
    shm = Path("/dev/shm")
    if not shm.is_dir() or os.stat(shm).st_dev == os.stat(tmp_path).st_dev:
        pytest.skip("no other file system at /dev/shm")
    src = tmp_path / "a.txt"
    src.write_text("alpha\n")
    dst = shm / f"rename-file-{os.getpid()}.txt"
    with pytest.raises(OSError) as err:
        rename_file(src, dst)
    assert err.value.errno == errno.EXDEV
    assert src.read_text() == "alpha\n"
    assert not dst.exists()
