# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

import pytest

from first_and_last_lines import first_and_last_lines

# name: (the bytes of the file, the count, the first lines, the last lines)
CASES = {
    "two": (b"a\nb\nc\nd\ne\n", 2, ["a", "b"], ["d", "e"]),
    "one": (b"first\nmiddle\nlast\n", 1, ["first"], ["last"]),
    "ten": ("".join(f"{i}\n" for i in range(1, 13)).encode(), 10,
            [str(i) for i in range(1, 11)], [str(i) for i in range(3, 13)]),
    "empty": (b"", 2, [], []),
    "zero": (b"a\nb\n", 0, [], []),
    "short": (b"a\nb\n", 5, ["a", "b"], ["a", "b"]),
    "exact": (b"a\nb\n", 2, ["a", "b"], ["a", "b"]),
    "overlap": (b"a\nb\nc\n", 2, ["a", "b"], ["b", "c"]),
    "one-line": (b"a\n", 1, ["a"], ["a"]),
    "empty-lines": (b"\n\na\n\n", 1, [""], [""]),
    "only-newline": (b"\n", 1, [""], [""]),
    "no-final-newline": (b"a\nb\nc", 1, ["a"], ["c"]),
    "crlf": (b"a\r\nb\r\n", 1, ["a\r"], ["b\r"]),
    "non-ascii": ("caf\u00e9\nna\u00efve\nzo\u00eb\n".encode(), 1, ["caf\u00e9"], ["zo\u00eb"]),
    # 100000 lines with the numbers 0 to 99999.
    "many-lines": ("".join(f"{i}\n" for i in range(100000)).encode(), 3,
                   ["0", "1", "2"], ["99997", "99998", "99999"]),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path, name):
    data, count, first, last = CASES[name]
    path = tmp_path / "input.txt"
    path.write_bytes(data)
    assert first_and_last_lines(path, count) == (first, last)


def test_not_found(tmp_path):
    with pytest.raises(FileNotFoundError):
        first_and_last_lines(tmp_path / "missing.txt", 1)
