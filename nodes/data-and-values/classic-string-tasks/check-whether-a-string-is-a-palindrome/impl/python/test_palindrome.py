# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

import pytest

from palindrome import is_palindrome

CASES = [
    ("racecar", "racecar", True),
    ("panama", "A man, a plan, a canal: Panama", True),
    ("hello", "hello", False),
    ("empty", "", True),
    ("only-punctuation", "?! ...", True),
    ("one-letter", "a", True),
    ("two-letters", "ab", False),
    ("mixed-case", "Aa", True),
    ("nixon", "No 'x' in Nixon", True),
    ("cat-i-saw", "Was it a car or a cat I saw?", True),
    ("digits", "12321", True),
    ("digits-not", "123", False),
    ("zero-p", "0P", False),
    ("race-a-car", "race a car", False),
    ("even-near", "abca", False),
]


@pytest.mark.parametrize(("name", "text", "want"), CASES, ids=[c[0] for c in CASES])
def test_is_palindrome(name, text, want):
    assert is_palindrome(text) is want
