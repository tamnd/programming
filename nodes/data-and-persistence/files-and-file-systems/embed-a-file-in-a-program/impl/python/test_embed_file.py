# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

import os
from importlib import resources
from pathlib import Path

import pytest

import embed_file
from embed_file import embedded_line

CASES = {
    "first-line": (1, "alpha"),
    "second-line": (2, "café"),
    "last-line": (4, "gamma"),
    "non-ascii-line": (2, "café"),
    "empty-line": (3, ""),
    "zero": (0, None),
    "negative": (-1, None),
    "past-the-end": (5, None),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(name: str) -> None:
    n, want = CASES[name]
    assert embedded_line(n) == want


def test_bytes_are_the_file() -> None:
    data = resources.files(embed_file.__name__).joinpath("lines.txt").read_bytes()
    assert data == "# SPDX-License-Identifier: MIT\nalpha\ncafé\n\ngamma\n".encode()
    assert len(data) == 50


def test_working_directory_does_not_matter(tmp_path: Path) -> None:
    embed_file._lines.cache_clear()
    old = os.getcwd()
    os.chdir(tmp_path)
    try:
        assert embedded_line(4) == "gamma"
    finally:
        os.chdir(old)
