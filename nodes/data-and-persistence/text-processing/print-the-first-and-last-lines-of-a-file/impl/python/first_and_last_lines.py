# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

"""Gets the first lines and the last lines of a text file in one pass."""

from collections import deque
from os import PathLike


def first_and_last_lines(path: str | PathLike[str], count: int) -> tuple[list[str], list[str]]:
    """Returns the first count lines and the last count lines of the UTF-8 file at path.

    If the file has fewer than count lines, each list has all the lines. A line
    ends at LF, and the LF is not part of it. A last line with no LF is a line
    too. Raises FileNotFoundError if the file does not exist.
    """
    first: list[str] = []
    # A deque with maxlen=count drops its oldest line when a new line comes, so
    # it holds the last count lines that it has seen.
    last: deque[str] = deque(maxlen=count)
    # With newline="\n", only LF ends a line, and a CR stays in the line.
    with open(path, encoding="utf-8", newline="\n") as f:
        for line in f:
            line = line.removesuffix("\n")
            if len(first) < count:
                first.append(line)
            last.append(line)
    return first, list(last)
