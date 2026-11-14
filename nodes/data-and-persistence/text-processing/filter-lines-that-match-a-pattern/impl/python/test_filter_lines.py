# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

import pytest

from filter_lines import filter_lines

# name: (the bytes of the file, the pattern, the result)
CASES = {
    "literal": (b"apple\nbanana\ncherry\n", "an", ["banana"]),
    "several": (b"cat\ndog\ncatalog\n", "cat", ["cat", "catalog"]),
    "start": (b"ab\nba\n", "^a", ["ab"]),
    "end": (b"ab\nba\n", "a$", ["ba"]),
    "dot": (b"abc\nac\naxc\n", "a.c", ["abc", "axc"]),
    "star": (b"ac\nabc\nabbc\nadc\n", "ab*c", ["ac", "abc", "abbc"]),
    "whole-line": (b"a\nab\nba\n", "^a$", ["a"]),
    "empty": (b"", "a", []),
    "no-match": (b"abc\n", "x", []),
    "empty-lines": (b"a\n\nb\n\n", "^$", ["", ""]),
    "match-all": (b"a\n\nb\n", ".*", ["a", "", "b"]),
    "duplicates": (b"a\nb\na\n", "a", ["a", "a"]),
    "no-final-newline": (b"a\nab", "b", ["ab"]),
    "crlf-end": (b"ab\r\nab\n", "b$", ["ab"]),
    "crlf-dot": (b"a\r\n", "^a.$", ["a\r"]),
    "case": (b"Apple\napple\n", "apple", ["apple"]),
    "non-ascii": ("caf\u00e9\ncafe\n".encode(), "\u00e9", ["caf\u00e9"]),
    # 100000 lines with the numbers 0 to 99999. Five of them hold only nines.
    "many-lines": ("".join(f"{i}\n" for i in range(100000)).encode(), "^9*$", ["9", "99", "999", "9999", "99999"]),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path, name):
    data, pattern, expect = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert filter_lines(path, pattern) == expect


def test_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        filter_lines(tmp_path / "missing.txt", "a")
