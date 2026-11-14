# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-lines-from-a-file/.

"""Removes lines from a file and writes the rest back under the same name."""

import os
import shutil
import tempfile
from os import PathLike


def remove_lines(path: str | PathLike[str], start: int, count: int) -> int:
    """Removes count lines from the file at path, from line start, and returns how many it removed.

    Line 1 is the first line. A line ends at LF, and the LF goes with the line.
    The function returns less than count if the file ends first. If it removes
    no line, it does not write the file. Otherwise it writes the rest of the
    file to a new file in the same directory, and os.replace puts the new file
    in the place of the old one. Raises ValueError if start is less than 1 or
    count is less than 0, and FileNotFoundError if the file does not exist.
    """
    if start < 1 or count < 0:
        raise ValueError(f"bad range: start {start}, count {count}")
    path = os.fspath(path)
    directory, name = os.path.split(path)
    # Binary mode: only LF ends a line, and the bytes do not change.
    with open(path, "rb") as src:
        fd, tmp = tempfile.mkstemp(prefix=f".{name}.", suffix=".tmp", dir=directory or ".")
        try:
            removed = 0
            with os.fdopen(fd, "wb") as dst:
                for number, line in enumerate(src, start=1):
                    if start <= number < start + count:
                        removed += 1
                    else:
                        dst.write(line)
                if removed > 0:
                    dst.flush()
                    os.fsync(dst.fileno())
            if removed == 0:
                os.remove(tmp)
                return 0
            # mkstemp makes the file readable and writable only by its owner.
            shutil.copymode(path, tmp)
            os.replace(tmp, path)
            return removed
        except BaseException:
            if os.path.exists(tmp):
                os.remove(tmp)
            raise
