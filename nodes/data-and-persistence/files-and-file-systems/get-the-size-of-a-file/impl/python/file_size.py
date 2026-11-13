# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-size-of-a-file/.

import errno
import os
import stat


def file_size(path: str | os.PathLike) -> int:
    """Returns the size in bytes of the regular file at path.

    os.stat follows a symbolic link, so a link gives the size of its target.
    A missing file raises FileNotFoundError. os.stat also gives a size for a
    directory, but it is not the size of a file, so the code raises
    IsADirectoryError.
    """
    st = os.stat(path)
    if stat.S_ISDIR(st.st_mode):
        raise IsADirectoryError(errno.EISDIR, os.strerror(errno.EISDIR), os.fspath(path))
    return st.st_size
