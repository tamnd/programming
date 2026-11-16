# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-anagrams/.

import pytest

from anagrams import are_anagrams

CASES = [
    ("listen-silent", "listen", "silent", True),
    ("dormitory", "Dormitory", "dirty room", True),
    ("hello-world", "hello", "world", False),
    ("both-empty", "", "", True),
    ("one-empty", "a", "", False),
    ("same-set-other-counts", "aab", "abb", False),
    ("extra-letter", "abc", "abcd", False),
    ("same-length-other-letters", "abc", "abd", False),
    ("case", "Listen", "SILENT", True),
    ("same-word", "stop", "stop", True),
    ("gentleman", "a gentleman", "elegant man", True),
    ("they-see", "The eyes!", "They see.", True),
    ("punctuation-only", "!?", "...", True),
]


@pytest.mark.parametrize(("name", "a", "b", "want"), CASES, ids=[c[0] for c in CASES])
def test_are_anagrams(name, a, b, want):
    assert are_anagrams(a, b) is want
    assert are_anagrams(b, a) is want
