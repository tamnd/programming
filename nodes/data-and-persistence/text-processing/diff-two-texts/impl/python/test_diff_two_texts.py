# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/diff-two-texts/.

import pytest

from diff_two_texts import diff_files, diff_lines

# name: (the old text, the new text, the edit script)
CASES = {
    "simple": ("a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n"),
    "change-a-line": ("one\ntwo\nthree\n", "one\n2\nthree\n", " one\n-two\n+2\n three\n"),
    "identical": ("a\nb\n", "a\nb\n", " a\n b\n"),
    "lines-of-a-poem": ("The cat sat\non the mat.\nIt was flat.\n", "The cat sat\non the hat.\nIt was flat.\nThe end.\n", " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n"),
    "both-empty": ("", "", ""),
    "old-empty": ("", "a\nb\n", "+a\n+b\n"),
    "new-empty": ("a\nb\n", "", "-a\n-b\n"),
    "all-different": ("a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n"),
    "insert-at-start": ("b\nc\n", "a\nb\nc\n", "+a\n b\n c\n"),
    "delete-at-end": ("a\nb\nc\n", "a\nb\n", " a\n b\n-c\n"),
    "swap": ("a\nb\n", "b\na\n", "-a\n b\n+a\n"),
    "repeated-lines": ("a\na\nb\n", "a\nb\nb\n", " a\n-a\n b\n+b\n"),
    "empty-line": ("a\n\nb\n", "a\nb\n", " a\n-\n b\n"),
    "no-final-newline": ("a\nb", "a\nb\n", " a\n b\n"),
    "crlf": ("a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n"),
    "trailing-space": ("a \nb\n", "a\nb\n", "-a \n+a\n b\n"),
    "normalization": ("caf\u00e9\n", "cafe\u0301\n", "-caf\u00e9\n+cafe\u0301\n"),
}


@pytest.mark.parametrize("name", CASES)
def test_diff_lines(name):
    old, new, want = CASES[name]
    assert diff_lines(old, new) == want


def test_many_lines():
    # 1000 lines, and 10 of them change.
    old = "".join(f"{i}\n" for i in range(1000))
    new = "".join((f"x{i}" if i % 100 == 50 else str(i)) + "\n" for i in range(1000))
    lines = diff_lines(old, new).splitlines()
    assert len(lines) == 1010
    assert lines[49:52] == [" 49", "-50", "+x50"]
    assert lines[-1] == " 999"


def test_diff_files(tmp_path):
    old, new, want = CASES["crlf"]
    # write_bytes keeps the CR, and write_text could change each LF.
    (tmp_path / "old.txt").write_bytes(old.encode())
    (tmp_path / "new.txt").write_bytes(new.encode())
    assert diff_files(tmp_path / "old.txt", tmp_path / "new.txt") == want


def test_not_found(tmp_path):
    (tmp_path / "new.txt").write_bytes(b"a\n")
    with pytest.raises(FileNotFoundError):
        diff_files(tmp_path / "missing.txt", tmp_path / "new.txt")
