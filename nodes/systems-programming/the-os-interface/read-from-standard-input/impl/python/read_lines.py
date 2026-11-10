# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-from-standard-input/.

import io
import sys
from typing import TextIO


def read_lines(stream: TextIO | None = None) -> list[str]:
    """Read a text stream to its end and return its lines without "\\n" or "\\r\\n".

    The default stream is standard input, decoded as UTF-8.
    """
    if stream is None:
        # The default encoding of sys.stdin comes from the locale.
        # reconfigure must come before the first read.
        if isinstance(sys.stdin, io.TextIOWrapper):
            sys.stdin.reconfigure(encoding="utf-8")
        stream = sys.stdin
    lines = []
    # The loop reads one line at a time and stops at the end of input.
    for line in stream:
        lines.append(line.removesuffix("\n").removesuffix("\r"))
    return lines


if __name__ == "__main__":
    for line in read_lines():
        print(line)
