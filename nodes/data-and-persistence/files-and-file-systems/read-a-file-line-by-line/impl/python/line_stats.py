# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-file-line-by-line/.

import os


def line_stats(path: str | os.PathLike[str]) -> tuple[int, int]:
    """Return the number of lines and the code points in the longest line."""
    lines = longest = 0
    # The default newline=None turns "\r\n" into "\n".
    with open(path, encoding="utf-8") as file:
        for line in file:
            lines += 1
            longest = max(longest, len(line.removesuffix("\n")))
    return lines, longest
