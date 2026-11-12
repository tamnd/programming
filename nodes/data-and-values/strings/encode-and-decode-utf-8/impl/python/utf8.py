# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/encode-and-decode-utf-8/.


def encode_utf8(cp: int) -> list[int]:
    """Return the UTF-8 bytes of the code point cp."""
    return list(chr(cp).encode("utf-8"))


def decode_utf8(b: list[int]) -> int:
    """Return the code point of the one UTF-8 sequence in b, or -1.

    The "strict" error handler raises UnicodeDecodeError for each sequence
    that is not well formed, so overlong forms and surrogates give -1.
    """
    try:
        text = bytes(b).decode("utf-8", errors="strict")
    except UnicodeDecodeError:
        return -1
    return ord(text) if len(text) == 1 else -1
