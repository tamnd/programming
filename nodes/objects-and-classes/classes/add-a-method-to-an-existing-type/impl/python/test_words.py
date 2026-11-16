# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

import pytest

from words import Text, word_count

CASES = {
    "three-words": ("the quick fox", 3),
    "one-word": ("hello", 1),
    "punctuation": ("Hello, world! How are you?", 5),
    "empty": ("", 0),
    "blank": (" \t\n ", 0),
    "mixed-whitespace": ("  two\twords\r\nand three  ", 4),
    "non-ascii": ("naïve café 東京", 3),
}


@pytest.mark.parametrize("name", CASES)
def test_word_count(name):
    text, want = CASES[name]
    assert word_count(text) == want


def test_text_is_still_a_str():
    t = Text("one two")
    assert isinstance(t, str)
    assert t == "one two"
    assert t.upper() == "ONE TWO"
    assert t.word_count() == 2


def test_str_itself_does_not_change():
    assert not hasattr("one two", "word_count")
    with pytest.raises(TypeError):
        str.word_count = Text.word_count
