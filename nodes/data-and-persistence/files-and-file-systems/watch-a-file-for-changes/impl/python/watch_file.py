# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/watch-a-file-for-changes/.
"""Watch a file for changes: poll its size and modification time."""

import errno
import os
import stat
import time

BASE_MTIME = 1_700_000_000  # seconds; the cases set the mtime to BASE_MTIME + step


def signature(path):
    """Return (size, mtime in ns) of the file at path, or None when it does not exist."""
    try:
        st = os.stat(path)
    except FileNotFoundError:
        return None
    if stat.S_ISDIR(st.st_mode):
        raise IsADirectoryError(errno.EISDIR, os.strerror(errno.EISDIR), os.fspath(path))
    return (st.st_size, st.st_mtime_ns)


class FileWatcher:
    """Remembers the last signature of a file and tells what changed since then."""

    def __init__(self, path):
        self.path = path
        self.last = signature(path)

    def poll(self):
        """Return created, deleted, modified or none, for the time since the last poll."""
        now = signature(self.path)
        before, self.last = self.last, now
        if before is None and now is None:
            return "none"
        if before is None:
            return "created"
        if now is None:
            return "deleted"
        return "modified" if now != before else "none"


def watch(path, on_change, interval=0.5, quiet=0.2, stop=lambda: False):
    """Call on_change(path) once for each burst of changes, after the file is quiet."""
    watcher = FileWatcher(path)
    pending = None  # the time of the last change that on_change has not seen
    while not stop():
        if watcher.poll() != "none":
            pending = time.monotonic()
        elif pending is not None and time.monotonic() - pending >= quiet:
            pending = None
            on_change(path)
        time.sleep(interval)


def apply_step(path, step, number):
    """Make the change of one step of a case (the entry of the cases uses it)."""
    kind, _, text = step.partition(":")
    if kind in ("create", "write"):
        with open(path, "w", encoding="utf-8", newline="") as f:
            f.write(text)
    elif kind == "append":
        with open(path, "a", encoding="utf-8", newline="") as f:
            f.write(text)
    elif kind == "replace":
        tmp = f"{path}.tmp"
        with open(tmp, "w", encoding="utf-8", newline="") as f:
            f.write(text)
        os.replace(tmp, path)
    elif kind == "recreate":
        os.remove(path)
        with open(path, "w", encoding="utf-8", newline="") as f:
            f.write(text)
    elif kind == "delete":
        os.remove(path)
    elif kind == "mkdir":
        os.mkdir(path)
    elif kind not in ("touch", "none"):
        raise ValueError(f"unknown step {step!r}")
    if kind != "none" and os.path.isfile(path):
        t = (BASE_MTIME + number) * 1_000_000_000
        os.utime(path, ns=(t, t))


def watch_changes(path, steps):
    """The entry of the cases: poll after each step and return the results."""
    watcher = FileWatcher(path)
    results = []
    for number, step in enumerate(steps, start=1):
        apply_step(path, step, number)
        results.append(watcher.poll())
    return results
