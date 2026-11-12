# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-file/.

import threading
from collections import Counter
from pathlib import Path

import pytest

from append_line import append_line

# name: (old content or None, line, content after)
CASES = {
    "new-file": (None, "first", "first\n"),
    "append-existing": ("one\n", "two", "one\ntwo\n"),
    "new-file-empty-line": (None, "", "\n"),
    "empty-line": ("a\n", "", "a\n\n"),
    "empty-file": ("", "x", "x\n"),
    "no-final-newline": ("one", "two", "onetwo\n"),
    "crlf-file": ("one\r\n", "two", "one\r\ntwo\n"),
    "whitespace": ("", "  two spaces\ttab ", "  two spaces\ttab \n"),
    "non-ascii": ("naïve\n", "café 日本語 🙂", "naïve\ncafé 日本語 🙂\n"),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    old, line, want = CASES[name]
    path = tmp_path / "log.txt"
    if old is not None:
        path.write_bytes(old.encode())
    append_line(path, line)
    assert path.read_bytes() == want.encode()


def test_missing_dir(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        append_line(tmp_path / "missing" / "log.txt", "x")


@pytest.mark.parametrize("line", ["a\nb", "a\rb"])
def test_invalid_line(tmp_path: Path, line: str) -> None:
    path = tmp_path / "log.txt"
    with pytest.raises(ValueError):
        append_line(path, line)
    assert not path.exists()


def test_many_writers(tmp_path: Path) -> None:
    path = tmp_path / "log.txt"
    writers, lines = 8, 250

    def write(w: int) -> None:
        for i in range(lines):
            append_line(path, f"writer {w} line {i:03d} " + "x" * 40)

    threads = [threading.Thread(target=write, args=(w,)) for w in range(writers)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    want = Counter(f"writer {w} line {i:03d} " + "x" * 40 for w in range(writers) for i in range(lines))
    assert Counter(path.read_text(encoding="utf-8").splitlines()) == want
