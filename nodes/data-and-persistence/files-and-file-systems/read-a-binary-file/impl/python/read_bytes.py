# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-binary-file/.

import os


def read_bytes(path: str | os.PathLike[str]) -> bytes:
    """Return all of the bytes of a file, with no decoding."""
    # "rb" is binary mode: no decoding and no change to line ends.
    with open(path, "rb") as file:
        return file.read()


def read_exact(path: str | os.PathLike[str], n: int) -> bytes:
    """Return the first n bytes of a file, or raise EOFError if it is shorter."""
    data = bytearray()
    with open(path, "rb") as file:
        # read(k) can give fewer than k bytes before the end of the file,
        # so read again until there are n bytes or read gives b"".
        while len(data) < n:
            chunk = file.read(n - len(data))
            if not chunk:
                raise EOFError(f"{os.fspath(path)}: {len(data)} of {n} bytes before the end of the file")
            data += chunk
    return bytes(data)
