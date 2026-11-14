# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

"""Counts the lines of a file without decoding it."""

from os import PathLike

CHUNK = 64 * 1024


def count_lines(path: str | PathLike[str]) -> tuple[int, int]:
    """Returns (newlines, lines) for the file at path.

    newlines is the number of LF bytes, the number that wc -l prints. lines
    also counts a last line that has no LF. Raises OSError, for example
    FileNotFoundError, if the file cannot be read.
    """
    newlines = 0
    last = b"\n"  # an empty file has no line without an LF
    with open(path, "rb") as f:  # binary mode: no decoding, any bytes are accepted
        while chunk := f.read(CHUNK):
            newlines += chunk.count(b"\n")
            last = chunk[-1:]
    return newlines, newlines + (last != b"\n")
