# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/lock-a-file/.

import fcntl
import os


class FileLock:
    """An flock lock on a whole file. Release it, or use it in a with statement."""

    def __init__(self, fd: int) -> None:
        self._fd = fd

    def release(self) -> None:
        if self._fd >= 0:
            fcntl.flock(self._fd, fcntl.LOCK_UN)
            os.close(self._fd)  # the close also releases the lock
            self._fd = -1

    def __enter__(self) -> "FileLock":
        return self

    def __exit__(self, *exc: object) -> None:
        self.release()


def try_lock_file(path: str | os.PathLike[str], mode: str) -> FileLock | None:
    """Ask for a shared or an exclusive lock on path, without a wait.

    It returns None when another open file has a lock that conflicts.
    """
    op = {"shared": fcntl.LOCK_SH, "exclusive": fcntl.LOCK_EX}[mode]
    # flock locks belong to the open file, so each call opens the file again.
    # The file must exist: O_CREAT is not given.
    fd = os.open(path, os.O_RDWR)
    try:
        fcntl.flock(fd, op | fcntl.LOCK_NB)
    except BlockingIOError:
        os.close(fd)
        return None
    except BaseException:
        os.close(fd)
        raise
    return FileLock(fd)


def try_lock(path: str | os.PathLike[str], held: str, want: str) -> bool:
    """The entry of the cases: take held, then ask for want."""
    first = None if held == "none" else try_lock_file(path, held)
    try:
        second = try_lock_file(path, want)
        if second is None:
            return False
        second.release()
        return True
    finally:
        if first is not None:
            first.release()
