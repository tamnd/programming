# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

"""Selects the lines of a text file that match a regular expression."""

import re
from os import PathLike


def filter_lines(path: str | PathLike[str], pattern: str) -> list[str]:
    """Returns the lines of the UTF-8 file at path that match pattern.

    A line matches if the pattern matches some part of it. A line ends at LF,
    and the LF is not part of it. A last line with no LF is a line too. Raises
    FileNotFoundError if the file does not exist, and re.error if the pattern
    is not valid.
    """
    regex = re.compile(pattern)
    # With newline="\n", only LF ends a line, and a CR stays in the line.
    with open(path, encoding="utf-8", newline="\n") as f:
        lines = (line.removesuffix("\n") for line in f)
        return [line for line in lines if regex.search(line)]
