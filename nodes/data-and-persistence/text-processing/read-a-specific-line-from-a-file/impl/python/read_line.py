# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

"""Reads one line of a text file by its number."""

from os import PathLike


def read_line(path: str | PathLike[str], number: int) -> str | None:
    """Returns line number of the UTF-8 file at path, or None if there is no such line.

    Line 1 is the first line. A line ends at LF, and the LF is not part of it.
    A last line with no LF is a line too. The function reads the file only up
    to the line. Raises FileNotFoundError if the file does not exist.
    """
    # With newline="\n", only LF ends a line, and a CR stays in the line.
    with open(path, encoding="utf-8", newline="\n") as f:
        if number < 1:
            return None
        for n, line in enumerate(f, start=1):
            if n == number:
                return line.removesuffix("\n")
    return None
