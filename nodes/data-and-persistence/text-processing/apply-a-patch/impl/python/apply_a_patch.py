# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/apply-a-patch/.

"""Applies a unified diff to a text, with no fuzz and no offset."""

import re
from os import PathLike

HUNK = re.compile(r"@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@")


class PatchError(ValueError):
    """The patch is malformed, or a hunk does not match the text.

    kind is "malformed" or "mismatch", and line is the number of the line of
    the patch where the error is.
    """

    def __init__(self, kind: str, line: int) -> None:
        super().__init__(f"{kind} at line {line} of the patch")
        self.kind = kind
        self.line = line


def split_lines(text: str) -> list[str]:
    """Splits text after each LF. Each line keeps its LF, and a last line
    with no LF is a line too."""
    lines = []
    start = 0
    while start < len(text):
        end = text.find("\n", start)
        end = len(text) if end < 0 else end + 1
        lines.append(text[start:end])
        start = end
    return lines


def apply_patch(old: str, patch: str) -> str:
    """Returns the text that the unified diff patch makes from the text old.

    Each hunk must match the lines of old at the start of its old range.
    Raises PatchError if the patch is malformed or a hunk does not match.
    """
    lines = split_lines(old)
    # The lines of the patch, with no LF. A CR stays in its line.
    p = [line.removesuffix("\n") for line in split_lines(patch)]
    out: list[str] = []
    cur = 0  # the index of the next line of old to use
    hunks = 0
    i = 0
    while i < len(p):
        if not p[i].startswith("@@"):
            i += 1  # a header line, or another line outside a hunk
            continue
        m = HUNK.match(p[i])
        if not m:
            raise PatchError("malformed", i + 1)
        hunks += 1
        old_start, old_count = int(m[1]), int(m[2] or 1)
        new_count = int(m[4] or 1)
        # An empty range starts after the line old_start.
        pos = old_start - 1 if old_count > 0 else old_start
        if pos < 0:
            raise PatchError("malformed", i + 1)
        if pos < cur or pos > len(lines):
            raise PatchError("mismatch", i + 1)
        out.extend(lines[cur:pos])
        cur = pos
        i += 1
        # The mark, the text and the number in the patch of each line, and
        # the end of each line: an LF, or "" before a backslash line.
        body: list[tuple[str, str, int]] = []
        ends: list[str] = []
        old_left, new_left = old_count, new_count
        while old_left > 0 or new_left > 0:
            if i == len(p):
                raise PatchError("malformed", i)
            line = p[i]
            mark, text = (line[0], line[1:]) if line else (" ", "")
            if mark == "\\":
                if not body:
                    raise PatchError("malformed", i + 1)
                ends[-1] = ""  # the line before has no LF
                i += 1
                continue
            if mark not in " -+":
                raise PatchError("malformed", i + 1)
            if mark in " -":
                old_left -= 1
            if mark in " +":
                new_left -= 1
            if old_left < 0 or new_left < 0:
                raise PatchError("malformed", i + 1)
            body.append((mark, text, i + 1))
            ends.append("\n")
            i += 1
        if i < len(p) and p[i].startswith("\\") and body:
            ends[-1] = ""
            i += 1
        for (mark, text, number), end in zip(body, ends):
            line = text + end
            if mark in " -":
                if cur == len(lines) or lines[cur] != line:
                    raise PatchError("mismatch", number)
                cur += 1
            if mark in " +":
                out.append(line)
    if hunks == 0:
        raise PatchError("malformed", len(p))
    out.extend(lines[cur:])
    return "".join(out)


def apply_patch_file(path: str | PathLike[str], patch_path: str | PathLike[str]) -> str:
    """Returns apply_patch of the two UTF-8 files.

    Raises FileNotFoundError if a file does not exist, and PatchError.
    """
    # Binary mode, then decode: a text mode file would change CRLF to LF.
    with open(path, "rb") as f:
        old = f.read().decode("utf-8")
    with open(patch_path, "rb") as f:
        patch = f.read().decode("utf-8")
    return apply_patch(old, patch)
