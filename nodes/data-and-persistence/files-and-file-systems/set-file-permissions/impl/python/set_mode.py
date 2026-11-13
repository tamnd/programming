# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/set-file-permissions/.

import os
import re
import stat

_MODE = re.compile(r"[0-7]{3,4}")


def set_mode(path: str | os.PathLike, mode: str) -> int:
    """Sets the mode of the file at path, and returns the new permission bits.

    mode is three or four octal digits, as for the chmod command. re.fullmatch
    checks the whole string, so "640\n" is not valid. os.chmod follows a
    symbolic link and sets the mode as it is: the umask does not apply. The
    result is read back with os.stat, because the system can ignore some bits.
    """
    if not isinstance(mode, str) or not _MODE.fullmatch(mode):
        raise ValueError(f"invalid mode: {mode!r}")
    os.chmod(path, int(mode, 8))
    return stat.S_IMODE(os.stat(path).st_mode)
