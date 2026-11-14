# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

"""Sorts the lines of a text file."""

from os import PathLike


def sort_lines(path: str | PathLike[str]) -> list[str]:
    """Returns the lines of the UTF-8 file at path, sorted by code point.

    A line ends at LF, and the LF is not part of it. A last line with no LF is
    a line too. Raises FileNotFoundError if the file does not exist.
    """
    # newline="" keeps each CR in the text. The split below uses only LF,
    # because str.splitlines also splits at other characters.
    with open(path, encoding="utf-8", newline="") as f:
        lines = f.read().split("\n")
    # After the LF at the end of the last line, split gives one empty string.
    # An empty file gives [""] too, which is no lines.
    if lines[-1] == "":
        lines.pop()
    # str compares by code point, which is the order of the UTF-8 bytes.
    lines.sort()
    return lines
