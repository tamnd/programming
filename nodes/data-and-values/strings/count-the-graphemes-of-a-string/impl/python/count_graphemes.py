# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.
"""Count the extended grapheme clusters of UAX #29 for the code points of the contract."""

CR, LF, CONTROL, EXTEND, ZWJ, RI, PICT, OTHER = range(8)


def _kind(c: int) -> int:
    """Return the Grapheme_Cluster_Break kind of c, or PICT for Extended_Pictographic."""
    if c == 0x0D:
        return CR
    if c == 0x0A:
        return LF
    if c < 0x20 or 0x7F <= c <= 0x9F or c == 0xAD:
        return CONTROL
    if 0x300 <= c <= 0x36F or 0xFE00 <= c <= 0xFE0F or 0x1F3FB <= c <= 0x1F3FF:
        return EXTEND
    if c == 0x200D:
        return ZWJ
    if 0x1F1E6 <= c <= 0x1F1FF:
        return RI
    if c in (0xA9, 0xAE, 0x2764) or 0x1F400 <= c <= 0x1F4FD or 0x1F600 <= c <= 0x1F64F:
        return PICT
    return OTHER


def count_graphemes(s: str) -> int:
    """Return the number of graphemes in s. len(s) counts code points."""
    n = 0
    prev = None
    ri_run = 0  # the number of regional indicators that end at prev
    pict = False  # prev ends Extended_Pictographic Extend*
    zwj_after_pict = False  # prev is a ZWJ after Extended_Pictographic Extend*
    for ch in s:
        k = _kind(ord(ch))
        if prev is None:
            join = False
        elif prev == CR and k == LF:
            join = True  # GB3
        elif prev in (CR, LF, CONTROL) or k in (CR, LF, CONTROL):
            join = False  # GB4, GB5
        elif k in (EXTEND, ZWJ):
            join = True  # GB9
        elif k == PICT and zwj_after_pict:
            join = True  # GB11
        elif k == RI and ri_run % 2 == 1:
            join = True  # GB12, GB13
        else:
            join = False  # GB999
        if not join:
            n += 1
        zwj_after_pict = k == ZWJ and pict
        pict = k == PICT or (k == EXTEND and pict)
        ri_run = ri_run + 1 if k == RI else 0
        prev = k
    return n
