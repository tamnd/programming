# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-word-count/.

from pathlib import Path

import pytest

from word_count import CHUNK, word_count

# name: (the text of the file, (lines, words, bytes))
CASES = {
    "simple": ("one two three\n", (1, 3, 14)),
    "multiple-spaces": ("  a   b  \n", (1, 2, 10)),
    "tabs-and-lines": ("a\tb\nc\n", (2, 3, 6)),
    "empty": ("", (0, 0, 0)),
    "only-white-space": (" \n\t\n", (2, 0, 4)),
    "no-final-newline": ("one two", (0, 2, 7)),
    "crlf": ("a b\r\nc\r\n", (2, 3, 8)),
    "non-ascii": ("na\u00efve caf\u00e9\n", (1, 2, 13)),
    "unicode-spaces": ("a\u00a0b\u3000c\u2028d\n", (1, 4, 13)),
    "not-white-space": ("a\u200bb\u00adc\ufeffd\n", (1, 1, 13)),
    "control-separators": ("a\x1cb\x1fc\n", (1, 1, 6)),
    "nel": ("a\u0085b\n", (1, 2, 5)),
    # 100000 lines: more than one chunk of 64 KiB.
    "many-lines": ("a b\n" * 100000, (100000, 200000, 400000)),
    # The chunk ends inside the 3 bytes of U+3000.
    "chunk-boundary": ("x" * (CHUNK - 1) + "\u3000y\n", (1, 2, CHUNK + 4)),
}

# name: the bytes of a file that is not valid UTF-8
INVALID = {
    "invalid-bytes": b"ok\xff\xfe\n",
    "truncated": b"ok \xe2\x82",
    "surrogate": b"a\xed\xa0\x80b\n",
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    text, want = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(text.encode("utf-8"))
    assert word_count(path) == want


@pytest.mark.parametrize("name", INVALID)
def test_invalid_utf8(tmp_path: Path, name: str) -> None:
    path = tmp_path / "input.txt"
    path.write_bytes(INVALID[name])
    with pytest.raises(UnicodeDecodeError):
        word_count(path)


def test_missing_file(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        word_count(tmp_path / "missing.txt")
