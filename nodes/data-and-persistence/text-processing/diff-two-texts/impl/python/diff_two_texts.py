# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/diff-two-texts/.

"""Compares two texts line by line, and gives a shortest edit script."""

from os import PathLike


def split_lines(text: str) -> list[str]:
    """Splits text at each LF. A final LF does not start a new line."""
    lines = text.split("\n")
    # "a\n".split("\n") is ["a", ""], and "".split("\n") is [""].
    if lines[-1] == "":
        lines.pop()
    return lines


def diff_lines(old: str, new: str) -> str:
    """Returns the edit script that changes the lines of old into the lines of new.

    Each line of the result is a space and a line that stays, a - and a line
    of old that goes, or a + and a line of new that comes, with an LF at the
    end. The script has the smallest number of - and + lines. When two
    scripts are equally short, a - comes before a +.
    """
    a, b = split_lines(old), split_lines(new)
    n, m = len(a), len(b)
    w = m + 1
    # lcs[i * w + j] is the length of a longest common subsequence of a[i:]
    # and b[j:]. Row n and column m stay 0.
    lcs = [0] * ((n + 1) * w)
    for i in range(n - 1, -1, -1):
        for j in range(m - 1, -1, -1):
            if a[i] == b[j]:
                lcs[i * w + j] = lcs[(i + 1) * w + j + 1] + 1
            else:
                lcs[i * w + j] = max(lcs[(i + 1) * w + j], lcs[i * w + j + 1])
    out = []
    i = j = 0
    while i < n and j < m:
        if a[i] == b[j]:
            out.append(" " + a[i])
            i += 1
            j += 1
        elif lcs[(i + 1) * w + j] >= lcs[i * w + j + 1]:
            out.append("-" + a[i])
            i += 1
        else:
            out.append("+" + b[j])
            j += 1
    out.extend("-" + line for line in a[i:])
    out.extend("+" + line for line in b[j:])
    return "".join(line + "\n" for line in out)


def diff_files(old_path: str | PathLike[str], new_path: str | PathLike[str]) -> str:
    """Returns diff_lines of the two UTF-8 files.

    Raises FileNotFoundError if a file does not exist, and UnicodeDecodeError
    if a file is not valid UTF-8.
    """
    # Binary mode, then decode: a text mode file would change CRLF to LF.
    with open(old_path, "rb") as f:
        old = f.read().decode("utf-8")
    with open(new_path, "rb") as f:
        new = f.read().decode("utf-8")
    return diff_lines(old, new)
