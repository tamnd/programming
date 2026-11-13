# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/normalize-unicode-text/.

import pytest

from normalize import normalize

# U+00E9 is NFC, and e U+0301 is NFD. Both are the same text.
CASES = {
    "nfc-composes": ("e\u0301", "NFC", "\u00e9"),
    "nfd-decomposes": ("\u00e9", "NFD", "e\u0301"),
    "ascii": ("abc", "NFC", "abc"),
    "empty": ("", "NFD", ""),
    "already-nfc": ("\u00e9", "NFC", "\u00e9"),
    "already-nfd": ("e\u0301", "NFD", "e\u0301"),
    "mixed-nfc": ("cafe\u0301 caf\u00e9", "NFC", "caf\u00e9 caf\u00e9"),
    "mixed-nfd": ("cafe\u0301 caf\u00e9", "NFD", "cafe\u0301 cafe\u0301"),
    "upper-grave": ("E\u0300", "NFC", "\u00c8"),
    "diaeresis": ("u\u0308ber", "NFC", "\u00fcber"),
    "circumflex": ("\u00ee", "NFD", "i\u0302"),
    "umlauts": ("\u00c4\u00d6\u00dc\u00e4\u00f6\u00fc", "NFD", "A\u0308O\u0308U\u0308a\u0308o\u0308u\u0308"),
    "spacing-accent": ("\u00b4e", "NFC", "\u00b4e"),
    "no-break-space": ("a\u00a0b", "NFD", "a\u00a0b"),
    "sharp-s": ("stra\u00dfe", "NFD", "stra\u00dfe"),
}


@pytest.mark.parametrize("name", CASES)
def test_normalize(name):
    s, form, want = CASES[name]
    assert normalize(s, form) == want
