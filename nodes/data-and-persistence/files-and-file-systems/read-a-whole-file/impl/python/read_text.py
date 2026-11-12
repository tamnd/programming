# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-whole-file/.

import os


def read_text(path: str | os.PathLike[str]) -> str:
    """Return all of the text of a UTF-8 file, with no change to it."""
    # newline="" keeps "\r\n" as it is; the default would give "\n".
    # The codec "utf-8" keeps a byte order mark as U+FEFF.
    with open(path, encoding="utf-8", newline="") as file:
        return file.read()
