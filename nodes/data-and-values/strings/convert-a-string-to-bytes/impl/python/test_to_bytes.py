# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

import pytest

from to_bytes import to_bytes

# The encoding is UTF-8. U+00E9 is 2 bytes, U+20AC is 3 bytes and U+1F642 is 4 bytes. In Latin-1, U+00FF would be 1 byte.
CASES = {
    "ascii": ("abc", [97, 98, 99]),
    "letter": ("A", [65]),
    "punct": ("hi!", [104, 105, 33]),
    "empty": ("", []),
    "control": ("\t\n", [9, 10]),
    "two-byte": ("\u00e9", [195, 169]),
    "latin-1-max": ("\u00ff", [195, 191]),
    "three-byte": ("\u20ac", [226, 130, 172]),
    "bom": ("\ufeff", [239, 187, 191]),
    "astral": ("\U0001f642", [240, 159, 153, 130]),
    "combining": ("e\u0301", [101, 204, 129]),
    "mixed": ("a\u00e9\U0001f642", [97, 195, 169, 240, 159, 153, 130]),
}


@pytest.mark.parametrize("name", CASES)
def test_to_bytes(name):
    s, want = CASES[name]
    assert to_bytes(s) == bytes(want)
