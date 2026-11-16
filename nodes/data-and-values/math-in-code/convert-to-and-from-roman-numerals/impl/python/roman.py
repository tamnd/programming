# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

TABLE = (
    (1000, "M"), (900, "CM"), (500, "D"), (400, "CD"),
    (100, "C"), (90, "XC"), (50, "L"), (40, "XL"),
    (10, "X"), (9, "IX"), (5, "V"), (4, "IV"), (1, "I"),
)
VALUES = {"I": 1, "V": 5, "X": 10, "L": 50, "C": 100, "D": 500, "M": 1000}


def to_roman(n: int) -> str:
    """Return the Roman numeral of n, for 1 <= n <= 3999."""
    if not 1 <= n <= 3999:
        raise ValueError("n is out of range")
    parts = []
    for value, symbol in TABLE:
        while n >= value:
            parts.append(symbol)
            n -= value
    return "".join(parts)


def from_roman(s: str) -> int:
    """Return the value of a Roman numeral in the standard form."""
    if not s or any(c not in VALUES for c in s):
        raise ValueError("not a Roman numeral")
    total = 0
    for i, c in enumerate(s):
        value = VALUES[c]
        if i + 1 < len(s) and value < VALUES[s[i + 1]]:
            total -= value
        else:
            total += value
    if not 1 <= total <= 3999 or to_roman(total) != s:
        raise ValueError("not in the standard form")
    return total
