# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicate-lines/.

import pytest

from remove_duplicate_lines import remove_duplicate_lines

# name: (the bytes of the file, the result)
CASES = {
    "simple": (b"b\na\nb\nc\na\n", ["b", "a", "c"]),
    "adjacent": (b"a\na\nb\n", ["a", "b"]),
    "no-duplicates": (b"c\nb\na\n", ["c", "b", "a"]),
    "empty": (b"", []),
    "empty-lines": (b"\na\n\n\n", ["", "a"]),
    "all-same": (b"x\nx\nx\n", ["x"]),
    "no-final-newline": (b"a\nb\na", ["a", "b"]),
    "crlf": (b"a\r\na\n", ["a\r", "a"]),
    "case": (b"A\na\nA\n", ["A", "a"]),
    "whitespace": (b"a \na\n a\na \n", ["a ", "a", " a"]),
    # U+00E9, then e and U+0301: the same letter, but not the same code points.
    "normalization": ("\u00e9\ne\u0301\n\u00e9\n".encode(), ["\u00e9", "e\u0301"]),
    # 100000 lines with 1000 different lines.
    "many-lines": ("".join(f"{i % 1000}\n" for i in range(100000)).encode(), [str(i) for i in range(1000)]),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path, name):
    data, expect = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert remove_duplicate_lines(path) == expect


def test_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        remove_duplicate_lines(tmp_path / "missing.txt")
