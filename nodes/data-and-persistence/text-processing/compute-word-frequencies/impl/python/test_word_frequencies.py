# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-word-frequencies/.

import pytest

from word_frequencies import word_frequencies

# name: (the bytes of the file, n, the result)
CASES = {
    "simple": (b"the cat and the hat\n", 2, [("the", 2), ("and", 1)]),
    "case": (b"The THE the tHe\n", 5, [("the", 4)]),
    "punctuation": (b"it's well-dressed, isn't it?\n", 10,
                    [("it", 2), ("dressed", 1), ("isn", 1), ("s", 1), ("t", 1), ("well", 1)]),
    "empty": (b"", 3, []),
    "no-letters": (b"123 ... 42\n-- !!\n", 3, []),
    "ties": (b"b a c b a c\n", 2, [("a", 2), ("b", 2)]),
    "digits": (b"abc123def 42 x9\n", 10, [("abc", 1), ("def", 1), ("x", 1)]),
    "n-zero": (b"the cat and the hat\n", 0, []),
    "n-above-count": (b"x y x\n", 10, [("x", 2), ("y", 1)]),
    "crlf": (b"one two\r\ntwo\r\n", 5, [("two", 2), ("one", 1)]),
    "non-ascii": ("caf\u00e9 na\u00efve cafe\n".encode(), 10, [("caf", 1), ("cafe", 1), ("na", 1), ("ve", 1)]),
    "leading-separators": (b"  ...hello, world. Hello\n", 5, [("hello", 2), ("world", 1)]),
    "no-final-newline": (b"dog cat dog", 5, [("dog", 2), ("cat", 1)]),
    "not-utf8": (b"ok \xff\xfe ok\n", 5, [("ok", 2)]),
    "many-lines": (b"a b a\n" * 100000, 3, [("a", 200000), ("b", 100000)]),
    # One line of 100000 words: longer than one buffer of 64 KiB.
    "long-line": (b"word " * 100000, 3, [("word", 100000)]),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path, name):
    data, n, expect = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert word_frequencies(path, n) == expect


def test_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        word_frequencies(tmp_path / "missing.txt", 3)
