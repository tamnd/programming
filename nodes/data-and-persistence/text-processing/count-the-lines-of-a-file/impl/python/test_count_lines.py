# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

from pathlib import Path

import pytest

from count_lines import CHUNK, count_lines

# name: (the bytes of the file, (newlines, lines))
CASES = {
    "lf": (b"alpha\nbeta\ngamma\n", (3, 3)),
    "blank-lines": (b"first\n\n\nlast\n", (4, 4)),
    "empty": (b"", (0, 0)),
    "only-newline": (b"\n", (1, 1)),
    "no-final-newline": (b"one\ntwo\nthree", (2, 3)),
    "one-line-no-newline": (b"x", (0, 1)),
    "crlf": (b"one\r\ntwo\r\n", (2, 2)),
    "crlf-no-final-newline": (b"one\r\ntwo", (1, 2)),
    "cr-only": (b"one\rtwo\r", (0, 1)),
    "other-breaks": ("a\vb\fc\x85d\u2028e\n".encode(), (1, 1)),
    "invalid-utf8": (b"ok\n\xff\xfe\n", (2, 2)),
    "nul-bytes": (b"a\x00b\n\x00\n", (2, 2)),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    data, want = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert count_lines(path) == want


def test_many_chunks(tmp_path: Path) -> None:
    # 100000 lines of 2 bytes and a last line with no LF: more than one chunk.
    path = tmp_path / "input.txt"
    path.write_bytes(b"x\n" * 100000 + b"end")
    assert 2 * 100000 > CHUNK
    assert count_lines(path) == (100000, 100001)


def test_lf_at_chunk_end(tmp_path: Path) -> None:
    # The file ends with the LF that is the last byte of a full chunk.
    path = tmp_path / "input.txt"
    path.write_bytes(b"x" * (CHUNK - 1) + b"\n")
    assert count_lines(path) == (1, 1)


def test_missing_file(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        count_lines(tmp_path / "missing.txt")
