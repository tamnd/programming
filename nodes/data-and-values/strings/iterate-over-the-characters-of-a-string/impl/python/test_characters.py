# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

import pytest

from characters import characters

# A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
CASES = {
    "ascii": ("abc", ["a", "b", "c"]),
    "word": ("hello", ["h", "e", "l", "l", "o"]),
    "space": ("a b", ["a", " ", "b"]),
    "empty": ("", []),
    "single": ("x", ["x"]),
    "non-ascii": ("h\u00e9", ["h", "\u00e9"]),
    "cjk": ("\u65e5\u672c", ["\u65e5", "\u672c"]),
    "astral": ("a\U0001f642b", ["a", "\U0001f642", "b"]),
    "combining": ("e\u0301", ["e", "\u0301"]),
    "regional-indicator": ("\U0001f1fa\U0001f1f8", ["\U0001f1fa", "\U0001f1f8"]),
    "zwj": ("\U0001f469\u200d\U0001f4bb", ["\U0001f469", "\u200d", "\U0001f4bb"]),
    "control": ("a\tb\nc", ["a", "\t", "b", "\n", "c"]),
}


@pytest.mark.parametrize("name", CASES)
def test_characters(name):
    s, want = CASES[name]
    assert characters(s) == want
