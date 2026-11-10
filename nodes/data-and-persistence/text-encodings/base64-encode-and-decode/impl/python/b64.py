# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/base64-encode-and-decode/.

import base64


def encode(data: bytes, alphabet: str = "standard") -> str:
    """Encode data as Base64 text.

    The standard alphabet pads the text with "=". The url alphabet uses
    "-" and "_" for "+" and "/" and adds no padding.
    """
    if alphabet == "standard":
        return base64.b64encode(data).decode("ascii")
    if alphabet == "url":
        return base64.urlsafe_b64encode(data).decode("ascii").rstrip("=")
    raise ValueError(f"unknown alphabet: {alphabet!r}")


def decode(text: str, alphabet: str = "standard") -> bytes:
    """Decode Base64 text strictly, and raise ValueError on bad input.

    The text is valid only when it is exactly what encode gives for some
    bytes: no white space, the right padding, and zero pad bits.
    """
    if alphabet not in ("standard", "url"):
        raise ValueError(f"unknown alphabet: {alphabet!r}")
    try:
        if alphabet == "url":
            padded = text + "=" * (-len(text) % 4)
            data = base64.b64decode(padded, altchars=b"-_", validate=True)
        else:
            data = base64.b64decode(text, validate=True)
    except ValueError:  # binascii.Error, or a non-ASCII character
        raise ValueError(f"invalid base64 text: {text!r}") from None
    # validate=True still accepts non-zero pad bits, and with altchars it
    # also accepts "+" and "/". Only the canonical text encodes back to
    # itself, so this check rejects both.
    if encode(data, alphabet) != text:
        raise ValueError(f"invalid base64 text: {text!r}")
    return data
