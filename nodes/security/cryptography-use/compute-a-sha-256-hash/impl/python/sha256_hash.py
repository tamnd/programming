# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

import hashlib
import os


def sha256_text(text: str) -> str:
    """Return the SHA-256 of the UTF-8 bytes of text, as lowercase hex."""
    return hashlib.sha256(text.encode("utf-8")).hexdigest()


def sha256_file(path: str | os.PathLike[str]) -> str:
    """Return the SHA-256 of the bytes of the file at path, as lowercase hex.

    The file is read in chunks, so a large file does not fill the memory.
    """
    # file_digest needs a file opened in binary mode.
    with open(path, "rb") as file:
        return hashlib.file_digest(file, "sha256").hexdigest()
