# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-duration/.


def format_duration(seconds: int) -> str:
    """Return the duration as HH:MM:SS. The hours do not wrap at 24."""
    sign = "-" if seconds < 0 else ""
    minutes, s = divmod(abs(seconds), 60)
    hours, m = divmod(minutes, 60)
    return f"{sign}{hours:02d}:{m:02d}:{s:02d}"
