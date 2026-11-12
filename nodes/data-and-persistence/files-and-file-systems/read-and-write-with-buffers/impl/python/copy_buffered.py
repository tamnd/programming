# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-and-write-with-buffers/.

import io
import os


def copy_buffered(src: str | os.PathLike, dst: str | os.PathLike, size: int) -> int:
    """Copies src to dst through buffers of size bytes and returns the byte count.

    The with statement closes the writer, and the close flushes the buffer.
    An error from that last write comes out of the with statement.
    """
    if size < 1:
        raise ValueError(f"size must be at least 1, not {size}")
    total = 0
    with (
        io.BufferedReader(io.FileIO(src, "r"), buffer_size=size) as reader,
        io.BufferedWriter(io.FileIO(dst, "w"), buffer_size=size) as writer,
    ):
        # read1 makes at most one call to the file; b"" is the end of the file.
        while chunk := reader.read1(size):
            writer.write(chunk)
            total += len(chunk)
    return total
