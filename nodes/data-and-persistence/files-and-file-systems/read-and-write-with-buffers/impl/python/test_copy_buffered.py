# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-and-write-with-buffers/.

import errno
import os
from pathlib import Path

import pytest

from copy_buffered import copy_buffered

TEXT = b"alpha\nbeta\ngamma\n"
CASES = {
    "text": (TEXT, 4096),
    "several-buffers": (TEXT, 4),
    "empty": (b"", 8),
    "smaller-than-buffer": (b"12345", 8),
    "exact-buffer": (b"12345678", 8),
    "one-past-buffer": (b"123456789", 8),
    "size-one": (TEXT, 1),
    "binary": (bytes(range(256)), 64),
    "large": (bytes(range(256)) * 4096, 4096),
    "large-odd-buffer": (bytes(range(256)) * 4096, 4093),
}


@pytest.mark.parametrize("name", CASES)
def test_copy(tmp_path: Path, name: str) -> None:
    data, size = CASES[name]
    src, dst = tmp_path / "in.bin", tmp_path / "out.bin"
    src.write_bytes(data)
    assert copy_buffered(src, dst, size) == len(data)
    assert dst.read_bytes() == data


def test_replaces_a_longer_file(tmp_path: Path) -> None:
    src, dst = tmp_path / "in.bin", tmp_path / "out.bin"
    src.write_bytes(b"new")
    dst.write_bytes(b"old and longer")
    assert copy_buffered(src, dst, 8) == 3
    assert dst.read_bytes() == b"new"


def test_missing_src(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        copy_buffered(tmp_path / "missing.bin", tmp_path / "out.bin", 8)
    assert not (tmp_path / "out.bin").exists()


def test_missing_dst_dir(tmp_path: Path) -> None:
    src = tmp_path / "in.bin"
    src.write_bytes(TEXT)
    with pytest.raises(FileNotFoundError):
        copy_buffered(src, tmp_path / "no-such-dir" / "out.bin", 8)


@pytest.mark.parametrize("size", [0, -1])
def test_invalid_size(tmp_path: Path, size: int) -> None:
    src = tmp_path / "in.bin"
    src.write_bytes(TEXT)
    with pytest.raises(ValueError):
        copy_buffered(src, tmp_path / "out.bin", size)


@pytest.mark.skipif(not os.path.exists("/dev/full"), reason="no /dev/full")
@pytest.mark.parametrize("data", [TEXT, bytes(1 << 20)], ids=["at-close", "at-write"])
def test_full_device(tmp_path: Path, data: bytes) -> None:
    # 17 bytes stay in the buffer, so the error comes only at the close.
    src = tmp_path / "in.bin"
    src.write_bytes(data)
    with pytest.raises(OSError) as err:
        copy_buffered(src, "/dev/full", 4096)
    assert err.value.errno == errno.ENOSPC
