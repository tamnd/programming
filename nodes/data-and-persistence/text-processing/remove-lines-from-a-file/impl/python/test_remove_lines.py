# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-lines-from-a-file/.

import os
import stat

import pytest

from remove_lines import remove_lines

NUMBERS = "".join(f"{i}\n" for i in range(100000)).encode()

# name: (the bytes of the file, start, count, the result, the bytes after)
CASES = {
    "first-two": (b"a\nb\nc\nd\n", 1, 2, 2, b"c\nd\n"),
    "middle": (b"a\nb\nc\nd\n", 2, 2, 2, b"a\nd\n"),
    "last": (b"a\nb\nc\n", 3, 1, 1, b"a\nb\n"),
    "past-the-end": (b"a\nb\n", 3, 1, 0, b"a\nb\n"),
    "partly-past-the-end": (b"a\nb\nc\n", 2, 5, 2, b"a\n"),
    "count-zero": (b"a\nb\n", 1, 0, 0, b"a\nb\n"),
    "all-lines": (b"a\nb\n", 1, 2, 2, b""),
    "empty": (b"", 1, 1, 0, b""),
    "empty-line": (b"a\n\nb\n", 2, 1, 1, b"a\nb\n"),
    "no-final-newline": (b"a\nb", 1, 1, 1, b"b"),
    "remove-incomplete-line": (b"a\nb", 2, 1, 1, b"a\n"),
    "crlf": (b"a\r\nb\r\nc\r\n", 2, 1, 1, b"a\r\nc\r\n"),
    "lone-cr": (b"a\rb\nc\n", 1, 1, 1, b"c\n"),
    "non-ascii": ("caf\u00e9\nna\u00efve\nzo\u00eb\n".encode(), 2, 1, 1, "caf\u00e9\nzo\u00eb\n".encode()),
    "large-count": (b"a\nb\n", 2, 2147483647, 1, b"a\n"),
    # 100000 lines with the numbers 0 to 99999. Lines 2 to 99999 go.
    "many-lines": (NUMBERS, 2, 99998, 99998, b"0\n99999\n"),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path, name):
    data, start, count, expect, after = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert remove_lines(path, start, count) == expect
    assert path.read_bytes() == after
    # No temporary file stays in the directory.
    assert os.listdir(tmp_path) == ["input.txt"]


def test_no_change_keeps_the_file(tmp_path):
    path = tmp_path / "input.txt"
    path.write_bytes(b"a\n")
    before = os.stat(path).st_ino
    assert remove_lines(path, 2, 1) == 0
    assert os.stat(path).st_ino == before


@pytest.mark.skipif(os.name != "posix", reason="POSIX permission bits")
def test_keeps_the_permissions(tmp_path):
    path = tmp_path / "input.txt"
    path.write_bytes(b"a\nb\n")
    os.chmod(path, 0o640)
    assert remove_lines(path, 1, 1) == 1
    assert stat.S_IMODE(os.stat(path).st_mode) == 0o640


def test_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        remove_lines(tmp_path / "missing.txt", 1, 1)


@pytest.mark.parametrize("start, count", [(0, 1), (1, -1)])
def test_bad_range(tmp_path, start, count):
    path = tmp_path / "input.txt"
    path.write_bytes(b"a\nb\n")
    with pytest.raises(ValueError):
        remove_lines(path, start, count)
    assert path.read_bytes() == b"a\nb\n"
