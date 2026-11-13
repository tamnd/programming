# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/copy-a-file/.

import errno
import os
import shutil


def copy_file(src: str | os.PathLike, dst: str | os.PathLike) -> None:
    """Copies the bytes and the permission bits of the file at src to dst.

    shutil.copy uses the fast copy call of the platform, and it raises
    shutil.SameFileError when src and dst are the same file. But it copies
    into a directory at dst, so the function raises IsADirectoryError first.
    Each other error, for example a missing src, comes out as an OSError.
    """
    if os.path.isdir(dst):
        raise IsADirectoryError(errno.EISDIR, os.strerror(errno.EISDIR), os.fspath(dst))
    shutil.copy(src, dst)
