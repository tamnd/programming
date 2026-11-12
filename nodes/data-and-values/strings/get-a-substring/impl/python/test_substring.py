# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-substring/.

import pytest

from substring import substring

# U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
CASES = {
    "middle": ("Hello, world", 7, 12, "world"),
    "prefix": ("Hello, world", 0, 5, "Hello"),
    "one-char": ("abc", 1, 2, "b"),
    "empty": ("", 0, 0, ""),
    "whole": ("abc", 0, 3, "abc"),
    "empty-range": ("abc", 1, 1, ""),
    "at-end": ("abc", 3, 3, ""),
    "non-ascii": ("naïve café", 2, 5, "ïve"),
    "cjk": ("日本語テキスト", 3, 7, "テキスト"),
    "astral": ("a\U0001f642b\U0001f643c", 1, 4, "\U0001f642b\U0001f643"),
    "astral-only": ("\U0001f642\U0001f643", 1, 2, "\U0001f643"),
    "combining": ("e\u0301t\u00e9", 0, 1, "e"),
    "combining-mark": ("e\u0301t\u00e9", 1, 3, "\u0301t"),
}


@pytest.mark.parametrize("name", CASES)
def test_substring(name):
    s, start, end, want = CASES[name]
    assert substring(s, start, end) == want
