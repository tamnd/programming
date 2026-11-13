# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/normalize-unicode-text/.
"""Put a string in the normalization form NFC or NFD."""

import unicodedata


def normalize(s: str, form: str) -> str:
    """Return s in the form NFC or NFD. The 2 forms of the same text compare equal only after this."""
    return unicodedata.normalize(form, s)
