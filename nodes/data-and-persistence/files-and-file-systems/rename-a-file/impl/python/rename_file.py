# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rename-a-file/.

import os


def rename_file(src: str | os.PathLike, dst: str | os.PathLike) -> None:
    """Gives the file at src the name dst, and replaces a file at dst.

    os.replace is one rename call, so it is atomic. os.rename is not used,
    because on Windows it fails when dst exists. shutil.move is not used,
    because across file systems it copies the file, and a copy is not atomic.
    Each error, for example a cross-device error (EXDEV), comes out as an
    OSError.
    """
    os.replace(src, dst)
