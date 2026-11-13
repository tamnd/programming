# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

from functools import cache
from importlib import resources


@cache
def _lines() -> list[str]:
    """Reads lines.txt one time, from the place where this module is.

    Python has no step that puts a file into the program. The file ships
    next to the module, as package data, and importlib.resources finds it
    through the import system, so the working directory does not matter and
    a module in a zip file also works.
    """
    text = resources.files(__name__).joinpath("lines.txt").read_text(encoding="utf-8")
    # Line 0 is the license header of the file, and the text after the last
    # "\n" is empty.
    return text.split("\n")[1:-1]


def embedded_line(n: int) -> str | None:
    """Returns line n of lines.txt (1 is the line after the header), or None."""
    lines = _lines()
    return lines[n - 1] if 1 <= n <= len(lines) else None
