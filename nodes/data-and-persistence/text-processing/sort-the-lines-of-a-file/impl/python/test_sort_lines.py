# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

import pytest

from sort_lines import sort_lines

# name: (the bytes of the file, the result)
CASES = {
    "simple": (b"pear\napple\nfig\n", ["apple", "fig", "pear"]),
    "case": (b"b\nB\na\nA\n", ["A", "B", "a", "b"]),
    "numbers": (b"10\n9\n100\n", ["10", "100", "9"]),
    "empty": (b"", []),
    "one-empty-line": (b"\n", [""]),
    "empty-lines": (b"b\n\na\n\n", ["", "", "a", "b"]),
    "duplicates": (b"b\na\nb\n", ["a", "b", "b"]),
    "no-final-newline": (b"b\na", ["a", "b"]),
    "crlf": (b"b\r\na\r\n", ["a\r", "b\r"]),
    "prefix": (b"ab\na\n", ["a", "ab"]),
    "whitespace": (b"b\n a\n\tc\n", ["\tc", " a", "b"]),
    "non-ascii": ("\u00e9\nz\ne\n".encode(), ["e", "z", "\u00e9"]),
    # U+FF61 is below U+1F600, but its UTF-16 code unit is above 0xD83D.
    "supplementary": ("\U0001f600\n\uff61\n".encode(), ["\uff61", "\U0001f600"]),
    # str.splitlines splits at each of these characters, but they are not LF.
    "other-line-breaks": ("b\u2028a\nb\x1ca\nb\x0ca\n".encode(), ["b\x0ca", "b\x1ca", "b\u2028a"]),
    "many-lines": (b"b\na\n" * 50000, ["a"] * 50000 + ["b"] * 50000),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path, name):
    data, expect = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert sort_lines(path) == expect


def test_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        sort_lines(tmp_path / "missing.txt")
