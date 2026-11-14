# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-text-in-many-files/.

"""Replaces a text with another text in each file of a list."""

import os
import shutil
import tempfile
from collections.abc import Iterable
from os import PathLike


def replace_in_files(paths: Iterable[str | PathLike[str]], old: str, new: str) -> list[int]:
    """Replaces each instance of old with new in each file, and returns the number of instances in each file.

    old and new are plain text, not patterns. The function compares the UTF-8
    bytes of old with the bytes of each file, so it does not decode the file,
    and an instance can contain LF bytes. Instances do not overlap. A file with
    no instance is not written. A changed file goes to a new file in the same
    directory, and os.replace puts the new file in the place of the old one.
    The files are done in order. At the first error, the function stops: the
    files before have the new text, and the files after do not change. Raises
    ValueError if old is empty, and FileNotFoundError if a file does not exist.
    """
    if not old:
        raise ValueError("old is empty")
    old_bytes, new_bytes = old.encode(), new.encode()
    counts = []
    for path in paths:
        with open(path, "rb") as f:
            data = f.read()
        # bytes.count and bytes.replace both find instances that do not overlap,
        # from left to right, so the count is the number of replacements.
        n = data.count(old_bytes)
        if n > 0:
            write_atomically(path, data.replace(old_bytes, new_bytes))
        counts.append(n)
    return counts


def write_atomically(path: str | PathLike[str], data: bytes) -> None:
    """Writes data to a new file next to path, and renames it over path."""
    directory, name = os.path.split(os.fspath(path))
    fd, tmp = tempfile.mkstemp(prefix=f".{name}.", suffix=".tmp", dir=directory or ".")
    try:
        with os.fdopen(fd, "wb") as f:
            f.write(data)
            f.flush()
            os.fsync(f.fileno())
        # mkstemp makes the file readable and writable only by its owner.
        shutil.copymode(path, tmp)
        os.replace(tmp, path)
    except BaseException:
        if os.path.exists(tmp):
            os.remove(tmp)
        raise
