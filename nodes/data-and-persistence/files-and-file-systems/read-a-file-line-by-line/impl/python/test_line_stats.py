# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-file-line-by-line/.

from pathlib import Path

import pytest

from line_stats import line_stats

CASES = {
    "lf": (b"alpha\nbeta\ngamma\n", (3, 5)),
    "blank-lines": (b"first\n\n\nlast line\n", (4, 9)),
    "empty": (b"", (0, 0)),
    "only-newline": (b"\n", (1, 0)),
    "no-final-newline": (b"one\ntwo\nthree", (3, 5)),
    "crlf": (b"one\r\ntwo\r\nthree\r\n", (3, 5)),
    "crlf-no-final-newline": (b"one\r\ntwo\r\nthree", (3, 5)),
    "mixed-endings": (b"one\r\ntwo\nthree\r\n", (3, 5)),
    "whitespace": (b"  two spaces\ttab \n", (1, 17)),
    "backslashes": (b"C:\\temp\\new\n", (1, 11)),
    "long-line": (b"x" * (1 << 20) + b"\nend\n", (2, 1 << 20)),
    "non-ascii": ("naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n".encode(), (3, 10)),
}

INVALID_UTF8 = {
    "invalid-byte": b"ok\n\xff\xfe\n",
    "truncated-at-end": b"ok\ncaf\xc3",
    "encoded-surrogate": b"a\xed\xa0\x80b\n",
    "overlong": b"\xc0\xaf\n",
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    data, want = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert line_stats(path) == want


def test_missing_file(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        line_stats(tmp_path / "missing.txt")


@pytest.mark.parametrize("name", INVALID_UTF8)
def test_invalid_utf8(tmp_path: Path, name: str) -> None:
    path = tmp_path / "input.txt"
    path.write_bytes(INVALID_UTF8[name])
    with pytest.raises(UnicodeDecodeError):
        line_stats(path)
