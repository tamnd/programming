# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-whole-file/.

from pathlib import Path

import pytest

from read_text import read_text

CASES = {
    "lf": "alpha\nbeta\n",
    "paragraphs": "first line\n\nsecond paragraph\n",
    "non-ascii": "naïve café\n日本語\n🙂\n",
    "empty": "",
    "no-final-newline": "one\ntwo",
    "trailing-newlines": "end\n\n\n",
    "crlf": "one\r\ntwo\r\n",
    "mixed-endings": "lf\ncrlf\r\ncr\rend",
    "bom": "\ufeffhead\n",
    "whitespace": "  two spaces\ttab \n",
    "large": "0123456789abcdef" * 65536,
}

INVALID_UTF8 = {
    "invalid-byte": b"ok\n\xff\xfe\n",
    "truncated-at-end": b"ok\ncaf\xc3",
    "encoded-surrogate": b"a\xed\xa0\x80b\n",
    "overlong": b"\xc0\xaf\n",
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    path = tmp_path / "input.txt"
    path.write_bytes(CASES[name].encode())
    assert read_text(path) == CASES[name]


def test_missing_file(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        read_text(tmp_path / "missing.txt")


def test_directory(tmp_path: Path) -> None:
    with pytest.raises(OSError):
        read_text(tmp_path)


@pytest.mark.parametrize("name", INVALID_UTF8)
def test_invalid_utf8(tmp_path: Path, name: str) -> None:
    path = tmp_path / "input.txt"
    path.write_bytes(INVALID_UTF8[name])
    with pytest.raises(UnicodeDecodeError):
        read_text(path)
