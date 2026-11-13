# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/truncate-a-file/.

import os


def truncate_file(path: str | os.PathLike, length: int) -> None:
    """Sets the size of the existing file at path to length bytes.

    os.truncate cuts the bytes after length, or adds zero bytes up to length.
    It does not make a missing file. Each error comes out as an OSError:
    FileNotFoundError, IsADirectoryError, or EINVAL for a length under 0.
    """
    os.truncate(path, length)
