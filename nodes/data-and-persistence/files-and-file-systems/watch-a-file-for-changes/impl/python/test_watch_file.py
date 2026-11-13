# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/watch-a-file-for-changes/.

import os
import threading
import time
from pathlib import Path

import pytest

import watch_file as wf

CASES = {
    "create": (["create:hello\n"], ["created"]),
    "create-then-write": (["create:a\n", "write:a much longer line\n"], ["created", "modified"]),
    "create-then-delete": (["create:a\n", "delete"], ["created", "deleted"]),
    "append": (["create:a\n", "append:b\n", "append:c\n"], ["created", "modified", "modified"]),
    "no-change": (["create:a\n", "none", "none"], ["created", "none", "none"]),
    "same-size": (["create:abc\n", "write:xyz\n"], ["created", "modified"]),
    "touch-only": (["create:a\n", "touch"], ["created", "modified"]),
    "replace-by-rename": (["create:old\n", "replace:new\n"], ["created", "modified"]),
    "delete-and-create": (["create:a\n", "recreate:b\n"], ["created", "modified"]),
    "never-created": (["none"], ["none"]),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, monkeypatch: pytest.MonkeyPatch, name: str) -> None:
    monkeypatch.chdir(tmp_path)
    steps, expect = CASES[name]
    assert wf.watch_changes("watched.txt", steps) == expect


def test_directory_is_an_error(tmp_path: Path, monkeypatch: pytest.MonkeyPatch) -> None:
    monkeypatch.chdir(tmp_path)
    with pytest.raises(IsADirectoryError):
        wf.watch_changes("watched.txt", ["mkdir"])


def test_size_and_time_together(tmp_path: Path) -> None:
    path = tmp_path / "watched.txt"
    path.write_text("abc\n")
    os.utime(path, ns=(5_000_000_000, 5_000_000_000))
    watcher = wf.FileWatcher(path)
    # The same size and the same mtime: a poll cannot see the change.
    path.write_text("xyz\n")
    os.utime(path, ns=(5_000_000_000, 5_000_000_000))
    assert watcher.poll() == "none"
    path.write_text("xyz!\n")
    assert watcher.poll() == "modified"


def test_watch_calls_once_for_a_burst(tmp_path: Path) -> None:
    path = tmp_path / "watched.txt"
    calls = []
    done = threading.Event()
    t = threading.Thread(
        target=wf.watch,
        args=(path, calls.append),
        kwargs={"interval": 0.01, "quiet": 0.5, "stop": done.is_set},
    )
    t.start()
    for i in range(5):
        path.write_text("x" * (i + 1))
        time.sleep(0.02)
    deadline = time.monotonic() + 5
    while not calls and time.monotonic() < deadline:
        time.sleep(0.01)
    time.sleep(0.6)
    done.set()
    t.join()
    assert calls == [path]
