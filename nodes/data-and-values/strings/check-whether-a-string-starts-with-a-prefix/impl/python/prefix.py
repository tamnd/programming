# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.


def starts_with(s: str, prefix: str) -> bool:
    """Return True if s starts with prefix."""
    return s.startswith(prefix)


def ends_with(s: str, suffix: str) -> bool:
    """Return True if s ends with suffix."""
    return s.endswith(suffix)
