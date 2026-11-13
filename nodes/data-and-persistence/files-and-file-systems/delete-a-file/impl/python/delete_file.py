# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/delete-a-file/.

import errno
import os


def delete_file(path: str | os.PathLike) -> bool:
    """Deletes the file at path. Gives True if a file was there, False if not.

    os.remove does not delete a directory, and it deletes a symbolic link,
    not its target. Each error other than "no such entry" comes out as an
    OSError, so a permission error is not taken for a missing file.
    """
    try:
        os.remove(path)
    except OSError as err:
        if err.errno in (errno.ENOENT, errno.ENOTDIR):
            return False
        raise
    return True
