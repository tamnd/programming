# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

import pytest

from read_line import read_line

NUMBERS = "".join(f"{i}\n" for i in range(100000)).encode()

# name: (the bytes of the file, the line number, the result)
CASES = {
    "first": (b"a\nb\nc\n", 1, "a"),
    "middle": (b"a\nb\nc\n", 2, "b"),
    "last": (b"a\nb\nc\n", 3, "c"),
    "past-the-end": (b"a\nb\n", 3, None),
    "zero": (b"a\n", 0, None),
    "negative": (b"a\n", -1, None),
    "empty": (b"", 1, None),
    "empty-line": (b"a\n\nb\n", 2, ""),
    "only-newline": (b"\n", 1, ""),
    "no-final-newline": (b"a\nb", 2, "b"),
    "crlf": (b"a\r\nb\r\n", 1, "a\r"),
    "lone-cr": (b"a\rb\nc\n", 2, "c"),
    "non-ascii": ("caf\u00e9\nzo\u00eb\n".encode(), 2, "zo\u00eb"),
    # 100000 lines with the numbers 0 to 99999. Line n holds n - 1.
    "many-lines": (NUMBERS, 100000, "99999"),
    "many-lines-past-the-end": (NUMBERS, 100001, None),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path, name):
    data, number, expect = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert read_line(path, number) == expect


def test_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        read_line(tmp_path / "missing.txt", 1)
