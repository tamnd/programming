# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-stack/.

import pytest

from balanced import balanced

DEEP = "(" * 1000 + ")" * 1000

CASES = {
    "pair": ("()", True),
    "nested": ("{[()()]}", True),
    "code": ("f(a[0], {b: 1})", True),
    "crossed": ("([)]", False),
    "empty": ("", True),
    "no-brackets": ("abc", True),
    "unclosed": ("((", False),
    "extra-close": ("())", False),
    "close-first": (")(", False),
    "wrong-kind": ("(]", False),
    "deep": (DEEP, True),
}


@pytest.mark.parametrize("case", CASES)
def test_balanced(case):
    text, want = CASES[case]
    assert balanced(text) is want
