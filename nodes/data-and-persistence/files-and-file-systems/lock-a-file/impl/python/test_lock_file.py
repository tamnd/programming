# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/lock-a-file/.

import os
from pathlib import Path

import pytest

from lock_file import try_lock, try_lock_file

CASES = {
    "no-lock-exclusive": ("none", "exclusive", True),
    "no-lock-shared": ("none", "shared", True),
    "exclusive-with-exclusive": ("exclusive", "exclusive", False),
    "shared-with-shared": ("shared", "shared", True),
    "shared-with-exclusive": ("shared", "exclusive", False),
    "exclusive-with-shared": ("exclusive", "shared", False),
}


@pytest.fixture
def data(tmp_path: Path) -> Path:
    path = tmp_path / "data.txt"
    path.write_text("alpha\n")
    return path


@pytest.mark.parametrize("name", CASES)
def test_cases(data: Path, name: str) -> None:
    held, want, expect = CASES[name]
    assert try_lock(data, held, want) is expect
    assert data.read_text() == "alpha\n"


def test_release_frees_the_lock(data: Path) -> None:
    lock = try_lock_file(data, "exclusive")
    assert lock is not None
    assert try_lock_file(data, "exclusive") is None
    lock.release()
    again = try_lock_file(data, "exclusive")
    assert again is not None
    again.release()


def test_with_statement_frees_the_lock(data: Path) -> None:
    with try_lock_file(data, "exclusive") as lock:
        assert lock is not None
        assert try_lock_file(data, "shared") is None
    with try_lock_file(data, "shared") as again:
        assert again is not None


def test_child_process_sees_the_lock(data: Path) -> None:
    lock = try_lock_file(data, "exclusive")
    assert lock is not None
    pid = os.fork()
    if pid == 0:
        os._exit(0 if try_lock_file(data, "shared") is None else 1)
    _, status = os.waitpid(pid, 0)
    lock.release()
    assert os.waitstatus_to_exitcode(status) == 0


def test_missing_file(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        try_lock_file(tmp_path / "missing.txt", "exclusive")
    assert os.listdir(tmp_path) == []
