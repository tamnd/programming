# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-from-standard-input/.

import io
import json
import subprocess
import sys
from pathlib import Path

import pytest

from read_lines import read_lines

CASES = {
    "lf": (b"alpha\nbeta\ngamma\n", ["alpha", "beta", "gamma"]),
    "blank-lines": (b"first\n\n\nlast line\n", ["first", "", "", "last line"]),
    "empty": (b"", []),
    "only-newline": (b"\n", [""]),
    "no-final-newline": (b"one\ntwo\nthree", ["one", "two", "three"]),
    "crlf": (b"one\r\ntwo\r\nthree\r\n", ["one", "two", "three"]),
    "crlf-no-final-newline": (b"one\r\ntwo\r\nthree", ["one", "two", "three"]),
    "mixed-endings": (b"one\r\ntwo\nthree\r\n", ["one", "two", "three"]),
    "trailing-blank-line": (b"last\n\n", ["last", ""]),
    "whitespace": (b"  two spaces\ttab \n", ["  two spaces\ttab "]),
    "backslashes": (b"C:\\temp\\new\n", ["C:\\temp\\new"]),
    "non-ascii": (
        "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n".encode(),
        ["naïve café", "日本語のテキスト", "🙂🙂🙂🙂🙂🙂"],
    ),
    "long-line": (b"x" * (1 << 20) + b"\nend\n", ["x" * (1 << 20), "end"]),
}

# The child process prints the result as JSON, so the test can compare it.
CHILD = "import json, read_lines; print(json.dumps(read_lines.read_lines()))"


@pytest.mark.parametrize("name", CASES)
def test_stdin(name: str) -> None:
    data, want = CASES[name]
    # pytest replaces sys.stdin, so a child process reads the real standard input.
    # LC_ALL=C makes the default encoding of the child ASCII, so the test also checks reconfigure.
    result = subprocess.run(
        [sys.executable, "-c", CHILD],
        input=data,
        capture_output=True,
        check=True,
        cwd=Path(__file__).parent,
        env={"LC_ALL": "C", "PYTHONUTF8": "0"},
    )
    assert json.loads(result.stdout) == want


@pytest.mark.parametrize("name", CASES)
def test_stream(name: str) -> None:
    data, want = CASES[name]
    # newline="" keeps "\r\n", as sys.stdin does on a pipe.
    stream = io.TextIOWrapper(io.BytesIO(data), encoding="utf-8", newline="")
    assert read_lines(stream) == want
