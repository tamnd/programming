# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-a-substring/.

import pytest

from replace import replace, replace_first

# U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
ALL = {
    "all": ("a-b-c", "-", "+", "a+b+c"),
    "word": ("Hello, world", "world", "there", "Hello, there"),
    "none": ("abc", "x", "y", "abc"),
    "empty-s": ("", "a", "b", ""),
    "delete": ("a-b-c", "-", "", "abc"),
    "new-contains-old": ("aXa", "X", "XXX", "aXXXa"),
    "overlap": ("aaaa", "aa", "b", "bb"),
    "overlap-odd": ("aaa", "aa", "b", "ba"),
    "case-sensitive": ("Hello hello", "hello", "bye", "Hello bye"),
    "non-ascii": ("naïve caf\u00e9", "\u00e9", "e", "naïve cafe"),
    "astral": ("a\U0001f642b\U0001f642", "\U0001f642", "!", "a!b!"),
    "combining": ("cafe\u0301", "e", "E", "cafE\u0301"),
    "precomposed": ("caf\u00e9", "e", "E", "caf\u00e9"),
    "dot": ("a.b.c", ".", "-", "a-b-c"),
    "dollar": ("price", "price", "$&!", "$&!"),
    "ampersand": ("a-b", "-", "&", "a&b"),
    "backslash": ("a/b", "/", "\\", "a\\b"),
}
FIRST = {
    "first-only": ("a-b-c", "-", "+", "a+b-c"),
    "first-none": ("abc", "x", "y", "abc"),
    "first-overlap": ("aaa", "aa", "b", "ba"),
    "first-astral": ("\U0001f642\U0001f642", "\U0001f642", "x", "x\U0001f642"),
    "first-dollar": ("a.b", ".", "$&", "a$&b"),
}


@pytest.mark.parametrize("name", ALL)
def test_replace(name):
    s, old, new, want = ALL[name]
    assert replace(s, old, new) == want


@pytest.mark.parametrize("name", FIRST)
def test_replace_first(name):
    s, old, new, want = FIRST[name]
    assert replace_first(s, old, new) == want
