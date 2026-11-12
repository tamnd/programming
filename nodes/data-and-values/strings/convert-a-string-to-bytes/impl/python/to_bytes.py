# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-a-string-to-bytes/.


def to_bytes(s: str) -> bytes:
    """Return the UTF-8 bytes of s.

    The encoding is given each time, so the result does not depend on the
    platform.
    """
    return s.encode("utf-8")
