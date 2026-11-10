# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-an-integer/.

_DIGITS = "0123456789abcdefghijklmnopqrstuvwxyz"
_MIN = -(2**63)
_MAX = 2**63 - 1


def parse_integer(s: str, base: int = 10) -> int:
    """Parse s as a signed 64-bit integer in the given base, from 2 to 36.

    Raise ValueError when s is not an optional sign and ASCII digits of the
    base, and OverflowError when the value does not fit in 64 bits.
    """
    allowed = _DIGITS[:base] + _DIGITS[10:base].upper()
    digits = s[1:] if s.startswith(("+", "-")) else s
    # int() alone also accepts white space, underscores, a 0x prefix and
    # every Unicode decimal digit, so check the characters first.
    if not digits or any(c not in allowed for c in digits):
        raise ValueError(f"invalid integer for base {base}: {s!r}")
    # A 64-bit value has at most 64 significant digits in any base. The
    # check also keeps a long string away from the digit limit of int().
    significant = digits.lstrip("0") or "0"
    if len(significant) > 64:
        raise OverflowError(f"integer out of range: {s!r}")
    value = int(significant, base)
    if s.startswith("-"):
        value = -value
    if not _MIN <= value <= _MAX:
        raise OverflowError(f"integer out of range: {s!r}")
    return value
