# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-file/.

import os
import tempfile


def write_text(path: str | os.PathLike[str], text: str, mode: str) -> None:
    """Write text to path as UTF-8, with no change to the line ends.

    mode is "truncate", "append" or "atomic".
    """
    if mode == "atomic":
        _replace(path, text)
        return
    flags = {"truncate": "w", "append": "a"}[mode]
    # newline="" turns off the change of "\n" to os.linesep.
    with open(path, flags, encoding="utf-8", newline="") as file:
        file.write(text)


def _replace(path: str | os.PathLike[str], text: str) -> None:
    # The temporary file is in the same directory, so the rename does not
    # cross a file system.
    directory = os.path.dirname(os.path.abspath(path))
    fd, tmp = tempfile.mkstemp(dir=directory, prefix=".tmp-")
    try:
        with open(fd, "w", encoding="utf-8", newline="") as file:
            file.write(text)
            file.flush()
            os.fsync(file.fileno())
        os.replace(tmp, path)
    except BaseException:
        os.unlink(tmp)
        raise
