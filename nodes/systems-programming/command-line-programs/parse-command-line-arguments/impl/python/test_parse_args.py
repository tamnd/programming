# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-command-line-arguments/.

import pytest

from parse_args import Args, UsageError, parse_args

CASES = {
    "files-only": (["a.txt", "b.txt"], (False, 10, ["a.txt", "b.txt"])),
    "short-options": (["-v", "-n", "3", "a.txt"], (True, 3, ["a.txt"])),
    "long-options": (["--verbose", "--limit", "5", "a.txt", "b.txt"], (True, 5, ["a.txt", "b.txt"])),
    "no-args": ([], (False, 10, [])),
    "long-equals": (["--limit=7", "notes.md"], (False, 7, ["notes.md"])),
    "repeated": (["-n", "3", "--limit", "5", "a.txt"], (False, 5, ["a.txt"])),
    "zero": (["-n", "0", "a.txt"], (False, 0, ["a.txt"])),
    "end-of-options": (["-v", "--", "-n", "--limit=4"], (True, 10, ["-n", "--limit=4"])),
    "only-terminator": (["--"], (False, 10, [])),
    "second-terminator": (["--", "--"], (False, 10, ["--"])),
    "dash": (["-"], (False, 10, ["-"])),
    "dash-after-option": (["-n", "2", "-", "b.txt"], (False, 2, ["-", "b.txt"])),
    "empty-arg": ([""], (False, 10, [""])),
    "space": (["-v", "my file.txt"], (True, 10, ["my file.txt"])),
    "non-ascii": (["--limit=4", "日本語.txt", "café.md"], (False, 4, ["日本語.txt", "café.md"])),
}

ERRORS = {
    "unknown-short": ["-x", "a.txt"],
    "unknown-long": ["--color", "a.txt"],
    "missing-value": ["-n"],
    "missing-value-long": ["--limit"],
    "not-a-number": ["-n", "ten", "a.txt"],
    "not-a-number-long": ["--limit=ten"],
    "empty-value": ["--limit=", "a.txt"],
    "verbose-with-value": ["--verbose=yes"],
}


@pytest.mark.parametrize("name", CASES)
def test_cases(name):
    args, want = CASES[name]
    assert parse_args(args) == Args(*want)


@pytest.mark.parametrize("name", ERRORS)
def test_errors(name):
    with pytest.raises(UsageError):
        parse_args(ERRORS[name])


def test_abbreviation_is_unknown():
    with pytest.raises(UsageError):
        parse_args(["--lim", "3"])
