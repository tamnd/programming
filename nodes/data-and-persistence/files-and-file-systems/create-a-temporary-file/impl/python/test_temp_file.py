# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-temporary-file/.

import os
import stat
from pathlib import Path

import pytest

from temp_file import make_temp_file, use_temp_file

# name: (dir, prefix, data)
CASES = {
    "one-line": (".", "tmp-", "alpha\n"),
    "many-lines": (".", "report-", "alpha\nbeta\ngamma\n"),
    "in-subdirectory": ("sub", "tmp-", "alpha\n"),
    "empty-prefix": (".", "", "alpha\n"),
    "empty-data": (".", "tmp-", ""),
    "non-ascii-data": (".", "tmp-", "café 日本\n"),
    "prefix-with-spaces": (".", "my temp ", "alpha\n"),
    "non-ascii-prefix": (".", "café-", "alpha\n"),
}


@pytest.fixture
def root(tmp_path: Path) -> Path:
    (tmp_path / "sub").mkdir()
    return tmp_path


@pytest.mark.parametrize("name", CASES)
def test_cases(root: Path, name: str) -> None:
    rel, prefix, data = CASES[name]
    assert use_temp_file(root / rel, prefix, data) == data
    assert sorted(p.name for p in root.iterdir()) == ["sub"]
    assert list((root / "sub").iterdir()) == []


@pytest.mark.parametrize("prefix", ["tmp-", "", "my temp ", "café-"])
def test_make_temp_file(root: Path, prefix: str) -> None:
    path = Path(make_temp_file(root, prefix))
    assert path.parent == root
    assert path.name.startswith(prefix)
    assert len(path.name) >= len(prefix) + 6
    st = os.lstat(path)
    assert stat.S_ISREG(st.st_mode)
    assert stat.S_IMODE(st.st_mode) == 0o600
    assert st.st_size == 0


def test_two_calls_give_two_files(root: Path) -> None:
    a = make_temp_file(root, "tmp-")
    b = make_temp_file(root, "tmp-")
    assert a != b
    assert os.path.isfile(a) and os.path.isfile(b)


def test_missing_directory(root: Path) -> None:
    with pytest.raises(FileNotFoundError):
        use_temp_file(root / "nodir", "tmp-", "alpha\n")


def test_prefix_with_slash(root: Path) -> None:
    with pytest.raises(ValueError):
        use_temp_file(root, "sub/tmp-", "alpha\n")
    assert list((root / "sub").iterdir()) == []
