# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.


def is_palindrome(text: str) -> bool:
    """Return True if the ASCII letters and digits of text, in lower case,
    read the same backwards. Other characters do not count."""
    kept = [c.lower() for c in text if c.isascii() and c.isalnum()]
    return kept == kept[::-1]


if __name__ == "__main__":
    print(is_palindrome("A man, a plan, a canal: Panama"))
