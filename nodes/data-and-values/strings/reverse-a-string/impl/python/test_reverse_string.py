# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-string/.

import pytest

from reverse_string import reverse_code_points

CASES = {
    "ascii": ("hello", "olleh"),
    "two-words": ("abc def", "fed cba"),
    "palindrome": ("racecar", "racecar"),
    "empty": ("", ""),
    "single": ("a", "a"),
    "two-byte": ("naïve", "evïan"),
    "three-byte": ("日本語", "語本日"),
    "astral": ("a🙂b", "b🙂a"),
    "two-astral": ("🙂😀", "😀🙂"),
    "combining-mark": ("e\u0301x", "x\u0301e"),
    "flag": ("🇫🇷", "🇷🇫"),
}


@pytest.mark.parametrize("s, want", CASES.values(), ids=CASES.keys())
def test_reverse_code_points(s: str, want: str) -> None:
    assert reverse_code_points(s) == want


@pytest.mark.parametrize("s", [s for s, _ in CASES.values()], ids=CASES.keys())
def test_twice_gives_the_input(s: str) -> None:
    assert reverse_code_points(reverse_code_points(s)) == s
