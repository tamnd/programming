# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-file/.

import os
from pathlib import Path

import pytest

from write_text import write_text

# name: (text, mode, the file before the call or None, the file after the call)
CASES = {
    "new-file": ("alpha\nbeta\n", "truncate", None, "alpha\nbeta\n"),
    "append-existing": ("two\n", "append", "one\n", "one\ntwo\n"),
    "atomic-replace": ("new\n", "atomic", "old\n", "new\n"),
    "truncate-longer": ("short\n", "truncate", "a much longer old line\n", "short\n"),
    "empty-truncate": ("", "truncate", "old\n", ""),
    "empty-new": ("", "truncate", None, ""),
    "append-missing": ("first\n", "append", None, "first\n"),
    "append-no-newline": ("two", "append", "one", "onetwo"),
    "non-ascii": ("naïve café\n日本語\n🙂\n", "truncate", None, "naïve café\n日本語\n🙂\n"),
    "crlf": ("one\r\ntwo\r\n", "truncate", None, "one\r\ntwo\r\n"),
    "mixed-endings": ("lf\ncrlf\r\ncr\rend", "truncate", None, "lf\ncrlf\r\ncr\rend"),
    "backslashes": ("C:\\temp\\new\n", "truncate", None, "C:\\temp\\new\n"),
    "format-chars": ("-n 100% %s\n", "truncate", None, "-n 100% %s\n"),
    "atomic-new": ("new\n", "atomic", None, "new\n"),
    "atomic-empty": ("", "atomic", "old\n", ""),
    "atomic-non-ascii": ("naïve café\n日本語\n🙂\n", "atomic", "old\n", "naïve café\n日本語\n🙂\n"),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    text, mode, before, want = CASES[name]
    path = tmp_path / "out.txt"
    if before is not None:
        path.write_bytes(before.encode())
    write_text(path, text, mode)
    # The exact bytes: UTF-8, no byte order mark, no change to the line ends.
    assert path.read_bytes() == want.encode()
    # No temporary file stays in the directory.
    assert os.listdir(tmp_path) == ["out.txt"]


@pytest.mark.parametrize("mode", ["truncate", "append", "atomic"])
def test_missing_dir(tmp_path: Path, mode: str) -> None:
    with pytest.raises(FileNotFoundError):
        write_text(tmp_path / "missing" / "out.txt", "x\n", mode)
    assert os.listdir(tmp_path) == []


def test_atomic_onto_directory(tmp_path: Path) -> None:
    path = tmp_path / "out.txt"
    path.mkdir()
    with pytest.raises(OSError):
        write_text(path, "x\n", "atomic")
    assert path.is_dir()
    assert os.listdir(tmp_path) == ["out.txt"]
