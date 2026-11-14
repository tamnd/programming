# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicate-lines/.

"""Removes the duplicate lines of a text file."""

from os import PathLike


def remove_duplicate_lines(path: str | PathLike[str]) -> list[str]:
    """Returns the lines of the UTF-8 file at path, each one only once.

    Each line stays at the place of its first copy. A line ends at LF, and the
    LF is not part of it. A last line with no LF is a line too. Memory grows
    with the different lines, not with the size of the file. Raises
    FileNotFoundError if the file does not exist.
    """
    with open(path, "rb") as f:
        # A file in binary mode splits lines at LF only, and keeps each CR.
        # dict.fromkeys keeps the first copy of each line, at its first place.
        unique = dict.fromkeys(line.removesuffix(b"\n") for line in f)
    return [line.decode("utf-8") for line in unique]
