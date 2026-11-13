# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-file-exists/.

import errno
import os
import stat


def file_status(path: str | os.PathLike) -> str:
    """Tells what is at path: file, directory, other, missing or broken-link.

    os.path.exists is not used, because it gives False also for a permission
    error. Here each error other than "no such entry" comes out as an OSError.
    """
    try:
        mode = os.stat(path).st_mode  # follows symbolic links
    except OSError as err:
        if err.errno not in (errno.ENOENT, errno.ENOTDIR):
            raise
        try:
            if stat.S_ISLNK(os.lstat(path).st_mode):
                return "broken-link"
        except OSError:
            pass
        return "missing"
    if stat.S_ISREG(mode):
        return "file"
    if stat.S_ISDIR(mode):
        return "directory"
    return "other"
