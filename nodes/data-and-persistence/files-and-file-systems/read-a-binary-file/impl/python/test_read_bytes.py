# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-binary-file/.

import os
import threading
import time
from pathlib import Path

import pytest

from read_bytes import read_bytes, read_exact

CASES = {
    "signature": bytes.fromhex("89504e470d0a1a0a0000000d49484452"),
    "bytes": bytes.fromhex("0001027f80feff"),
    "empty": b"",
    "all-bytes": bytes(range(256)),
    "zeros": bytes(8),
    "crlf": b"a\r\nb\r\n",
    "ctrl-z": b"a\x1ab",
    "not-utf8": bytes.fromhex("c328fffe80"),
    "large": bytes(range(256)) * 4096,
}

EXACT = {
    "signature-only": ("signature", 8, bytes.fromhex("89504e470d0a1a0a")),
    "first-four": ("all-bytes", 4, b"\x00\x01\x02\x03"),
    "whole-file": ("zeros", 8, bytes(8)),
    "zero-count": ("empty", 0, b""),
    "across-ctrl-z": ("ctrl-z", 3, b"a\x1ab"),
    "large": ("large", 1 << 20, CASES["large"]),
}

SHORT = {
    "one-past-end": ("signature", 17),
    "empty-file": ("empty", 1),
}


def write(tmp_path: Path, data: bytes) -> Path:
    path = tmp_path / "input.bin"
    path.write_bytes(data)
    return path


@pytest.mark.parametrize("name", CASES)
def test_read_bytes(tmp_path: Path, name: str) -> None:
    assert read_bytes(write(tmp_path, CASES[name])) == CASES[name]


@pytest.mark.parametrize("name", EXACT)
def test_read_exact(tmp_path: Path, name: str) -> None:
    data, n, want = EXACT[name]
    assert read_exact(write(tmp_path, CASES[data]), n) == want


@pytest.mark.parametrize("name", SHORT)
def test_short_file(tmp_path: Path, name: str) -> None:
    data, n = SHORT[name]
    with pytest.raises(EOFError):
        read_exact(write(tmp_path, CASES[data]), n)


@pytest.mark.parametrize("read", [read_bytes, lambda p: read_exact(p, 1)])
def test_missing_file(tmp_path: Path, read) -> None:
    with pytest.raises(FileNotFoundError):
        read(tmp_path / "missing.bin")


@pytest.mark.parametrize("read", [read_bytes, lambda p: read_exact(p, 1)])
def test_directory(tmp_path: Path, read) -> None:
    with pytest.raises(IsADirectoryError):
        read(tmp_path)


def test_read_exact_reads_again(tmp_path: Path) -> None:
    # A pipe gives the bytes in two parts, so one read call gets only the
    # first part. read_exact must read again to get all 8 bytes.
    fifo = tmp_path / "fifo"
    os.mkfifo(fifo)

    def writer() -> None:
        with open(fifo, "wb", buffering=0) as out:
            out.write(b"abc")
            time.sleep(0.2)
            out.write(b"defgh")

    thread = threading.Thread(target=writer)
    thread.start()
    try:
        assert read_exact(fifo, 8) == b"abcdefgh"
    finally:
        thread.join()
