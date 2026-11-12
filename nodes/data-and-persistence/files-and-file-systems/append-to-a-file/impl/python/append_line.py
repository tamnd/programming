# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-file/.

import os


def append_line(path: str | os.PathLike[str], line: str) -> None:
    """Add line and one "\\n" to the end of the file, in one write call."""
    if "\n" in line or "\r" in line:
        raise ValueError("the line must not hold \\n or \\r")
    data = (line + "\n").encode("utf-8")
    # O_APPEND moves the offset to the end of the file before each write.
    # open(path, "a") also uses it, but its buffer can split a long line.
    fd = os.open(path, os.O_WRONLY | os.O_APPEND | os.O_CREAT, 0o666)
    try:
        written = os.write(fd, data)
    finally:
        os.close(fd)
    if written != len(data):
        raise OSError(f"short write: {written} of {len(data)} bytes")
