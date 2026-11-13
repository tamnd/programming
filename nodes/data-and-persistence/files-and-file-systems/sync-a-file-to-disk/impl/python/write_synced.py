# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sync-a-file-to-disk/.

import os
import sys


def _sync(fd: int) -> None:
    # On macOS, fsync sends the data to the drive, but the drive can keep it
    # in its cache. F_FULLFSYNC asks the drive to write its cache too. Some
    # file systems do not support it, so fsync is the fallback.
    if sys.platform == "darwin":
        import fcntl

        try:
            fcntl.fcntl(fd, fcntl.F_FULLFSYNC)
            return
        except OSError:
            pass
    os.fsync(fd)


def sync_directory(directory: str | os.PathLike[str]) -> None:
    """Sync the entries of a directory, so that a new name in it is on disk."""
    fd = os.open(directory, os.O_RDONLY)
    try:
        os.fsync(fd)
    finally:
        os.close(fd)


def write_synced(path: str | os.PathLike[str], text: str) -> None:
    """Write text to path as UTF-8, and return when it is on the disk."""
    # newline="" turns off the change of "\n" to os.linesep.
    with open(path, "w", encoding="utf-8", newline="") as f:
        f.write(text)
        f.flush()  # from the buffer of Python to the kernel
        _sync(f.fileno())  # from the kernel to the device
    # The close of the with statement raises its error, if there is one.
    sync_directory(os.path.dirname(os.path.abspath(path)))
