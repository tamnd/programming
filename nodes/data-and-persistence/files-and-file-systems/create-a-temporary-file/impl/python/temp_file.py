# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-temporary-file/.

import os
import tempfile


def make_temp_file(dir: str | os.PathLike, prefix: str) -> str:
    """Makes a new, empty file in dir whose name starts with prefix, and returns its path.

    tempfile.mkstemp makes the file with os.O_EXCL and the mode 0600, and it
    returns an open descriptor, which this function closes. A prefix with a
    slash would put the file in another directory, so it is an error.
    """
    if "/" in prefix or os.sep in prefix:
        raise ValueError(f"invalid prefix: {prefix!r}")
    fd, path = tempfile.mkstemp(prefix=prefix, dir=dir)
    os.close(fd)
    return path


def use_temp_file(dir: str | os.PathLike, prefix: str, data: str) -> str:
    """Writes data to a new temporary file, reads it back, removes the file, and returns the text."""
    path = make_temp_file(dir, prefix)
    try:
        with open(path, "w+", encoding="utf-8", newline="") as f:
            f.write(data)
            f.seek(0)
            return f.read()
    finally:
        os.remove(path)
