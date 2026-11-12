# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

import pytest

from prefix import ends_with, starts_with

# U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
STARTS = {
    "yes": ("Hello, world", "Hello", True),
    "no": ("Hello, world", "world", False),
    "whole": ("abc", "abc", True),
    "empty-prefix": ("abc", "", True),
    "empty-s": ("", "a", False),
    "both-empty": ("", "", True),
    "longer-prefix": ("ab", "abc", False),
    "case-sensitive": ("Hello", "hello", False),
    "non-ascii": ("caf\u00e9 au lait", "caf\u00e9", True),
    "cjk": ("日本語", "日本", True),
    "astral": ("\U0001f642\U0001f643", "\U0001f642", True),
    "astral-no": ("\U0001f642", "\U0001f643", False),
    "combining": ("cafe\u0301", "cafe", True),
    "precomposed": ("caf\u00e9", "cafe", False),
}
ENDS = {
    "ends-yes": ("report.txt", ".txt", True),
    "ends-no": ("report.txt", ".csv", False),
    "ends-empty": ("abc", "", True),
    "ends-longer": ("txt", "a.txt", False),
    "ends-case": ("A.TXT", ".txt", False),
    "ends-astral": ("a\U0001f642", "\U0001f642", True),
    "ends-combining": ("cafe\u0301", "e", False),
}


@pytest.mark.parametrize("name", STARTS)
def test_starts_with(name):
    s, prefix, want = STARTS[name]
    assert starts_with(s, prefix) is want


@pytest.mark.parametrize("name", ENDS)
def test_ends_with(name):
    s, suffix, want = ENDS[name]
    assert ends_with(s, suffix) is want
