# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sync-a-file-to-disk/.

import os
from pathlib import Path

import pytest

import write_synced as ws

# name: (text, the file before the call or None)
CASES = {
    "new-file": ("alpha\nbeta\n", None),
    "replace-longer": ("short\n", "a much longer old line\n"),
    "many-lines": ("one\ntwo\nthree\nfour\n", None),
    "empty-replace": ("", "old\n"),
    "empty-new": ("", None),
    "no-final-newline": ("last line", None),
    "non-ascii": ("naïve café\n日本語\n🙂\n", None),
    "crlf": ("one\r\ntwo\r\n", None),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    text, before = CASES[name]
    path = tmp_path / "out.txt"
    if before is not None:
        path.write_bytes(before.encode())
    ws.write_synced(path, text)
    assert path.read_bytes() == text.encode()
    assert os.listdir(tmp_path) == ["out.txt"]


def test_syncs_the_file_and_the_directory(tmp_path: Path, monkeypatch: pytest.MonkeyPatch) -> None:
    synced = []
    real = os.fsync

    def spy(fd: int) -> None:
        synced.append(os.path.isdir(f"/dev/fd/{fd}"))
        real(fd)

    monkeypatch.setattr(os, "fsync", spy)
    monkeypatch.setattr(ws.sys, "platform", "linux")  # take the fsync path on macOS too
    ws.write_synced(tmp_path / "out.txt", "x\n")
    # First the file, then its directory.
    assert synced == [False, True]


def test_sync_error_is_raised(tmp_path: Path, monkeypatch: pytest.MonkeyPatch) -> None:
    def fail(fd: int) -> None:
        raise OSError(5, "Input/output error")

    monkeypatch.setattr(os, "fsync", fail)
    monkeypatch.setattr(ws.sys, "platform", "linux")
    with pytest.raises(OSError):
        ws.write_synced(tmp_path / "out.txt", "x\n")


def test_missing_directory(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        ws.write_synced(tmp_path / "missing" / "out.txt", "x\n")
    assert os.listdir(tmp_path) == []


def test_onto_directory(tmp_path: Path) -> None:
    path = tmp_path / "out.txt"
    path.mkdir()
    with pytest.raises(IsADirectoryError):
        ws.write_synced(path, "x\n")
    assert path.is_dir()
