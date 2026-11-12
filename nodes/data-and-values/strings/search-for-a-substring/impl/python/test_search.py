# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-for-a-substring/.

import pytest

from search import find, find_last

# U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
FIRST = {
    "found": ("Hello, world", "world", 7),
    "first-of-two": ("abcabc", "bc", 1),
    "not-found": ("abc", "d", -1),
    "empty-sub": ("abc", "", 0),
    "empty-s": ("", "a", -1),
    "both-empty": ("", "", 0),
    "longer-sub": ("ab", "abc", -1),
    "overlap": ("aaaa", "aa", 0),
    "case-sensitive": ("Hello", "hello", -1),
    "non-ascii": ("naïve caf\u00e9", "caf\u00e9", 6),
    "cjk": ("日本語テキスト", "テキ", 3),
    "astral": ("a\U0001f642b\U0001f642c", "b", 2),
    "astral-sub": ("x\U0001f642y", "\U0001f642y", 1),
    "combining": ("cafe\u0301", "e", 3),
    "precomposed": ("cafe\u0301", "\u00e9", -1),
}
LAST = {
    "last-of-two": ("abcabc", "bc", 4),
    "last-not-found": ("abc", "d", -1),
    "last-empty-sub": ("abc", "", 3),
    "last-empty-s": ("", "", 0),
    "last-overlap": ("aaaa", "aa", 2),
    "last-astral": ("\U0001f642a\U0001f642a", "a", 3),
    "last-non-ascii": ("caf\u00e9 caf\u00e9", "\u00e9", 8),
}


@pytest.mark.parametrize("name", FIRST)
def test_find(name):
    s, sub, want = FIRST[name]
    assert find(s, sub) == want


@pytest.mark.parametrize("name", LAST)
def test_find_last(name):
    s, sub, want = LAST[name]
    assert find_last(s, sub) == want
