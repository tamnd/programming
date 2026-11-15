# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/tail-a-growing-file/.

import os
import time
from collections.abc import Callable, Iterator


class Follower:
    """Follows a file that grows, as tail -F does, one poll at a time.

    The follower keeps the file open, so it can read the rest of the file
    after a rename or a delete. It starts at the end of the file. Raises
    FileNotFoundError if no file is at path.
    """

    def __init__(self, path: str | os.PathLike[str]) -> None:
        self.path = path
        self.file = open(path, "rb")
        self.file.seek(0, os.SEEK_END)
        self.pending = b""  # the bytes after the last LF

    def poll(self) -> list[str]:
        """Returns the complete lines that were added since the last poll."""
        lines: list[str] = []
        if self.file is not None:
            # Look at the path before the read. Then the read gets each byte
            # that was written before a rename or a delete.
            try:
                now = os.stat(self.path)
            except FileNotFoundError:
                now = None
            open_ = os.fstat(self.file.fileno())
            if open_.st_size < self.file.tell():
                # The file is shorter than the offset, so it was truncated.
                self._flush(lines)
                self.file.seek(0)
            self._read(lines)
            if now is None or not os.path.samestat(now, open_):
                # The file was deleted or renamed. Its rest is read.
                self._flush(lines)
                self.file.close()
                self.file = None
        if self.file is None:
            try:
                self.file = open(self.path, "rb")
            except FileNotFoundError:
                return lines
            self._read(lines)
        return lines

    def close(self) -> None:
        if self.file is not None:
            self.file.close()
            self.file = None

    def _read(self, lines: list[str]) -> None:
        assert self.file is not None
        parts = (self.pending + self.file.read()).split(b"\n")
        self.pending = parts.pop()
        lines.extend(part.decode() for part in parts)

    def _flush(self, lines: list[str]) -> None:
        if self.pending:
            lines.append(self.pending.decode())
            self.pending = b""


def follow(path: str, interval: float = 0.5, stop: Callable[[], bool] = lambda: False) -> Iterator[str]:
    """Returns an iterator over each new line of the file at path.

    The follower starts now, at the end of the file. The iterator polls the
    file each interval seconds, and ends when stop returns True.
    """
    follower = Follower(path)  # not in the generator, which starts later
    return _lines(follower, interval, stop)


def _lines(follower: Follower, interval: float, stop: Callable[[], bool]) -> Iterator[str]:
    try:
        while not stop():
            yield from follower.poll()
            time.sleep(interval)
    finally:
        follower.close()


def follow_file(path: str, initial: str, steps: list[str]) -> list[list[str]]:
    """Writes initial to path, makes a follower and does the steps."""
    with open(path, "wb") as f:
        f.write(initial.encode())
    follower = Follower(path)
    old = path + ".1"
    polls = []
    try:
        for step in steps:
            op, _, text = step.partition(":")
            if op == "poll":
                polls.append(follower.poll())
            elif op in ("append", "append-old"):
                with open(path if op == "append" else old, "ab") as f:
                    f.write(text.encode())
            elif op == "truncate":
                with open(path, "wb") as f:
                    f.write(text.encode())
            elif op == "rotate":
                os.replace(path, old)
                open(path, "xb").close()
            elif op == "delete":
                os.remove(path)
            elif op == "create":
                with open(path, "xb") as f:
                    f.write(text.encode())
            else:
                raise ValueError(f"bad step: {step!r}")
    finally:
        follower.close()
    return polls
