# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

import os


def file_mtime(path: str | os.PathLike) -> int:
    """Returns the modification time of the file at path, in whole seconds
    since 1970-01-01T00:00:00Z.

    os.stat follows a symbolic link. st_mtime_ns is an exact integer, so the
    floor division gives the correct second, also before 1970. A missing
    file raises FileNotFoundError.
    """
    return os.stat(path).st_mtime_ns // 1_000_000_000
