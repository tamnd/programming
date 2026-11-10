# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

import re


def date_pattern(sep: str) -> re.Pattern[str]:
    """Return the pattern of a date YYYY<sep>MM<sep>DD with no digit next to it."""
    # re.escape makes each special character of sep match only itself.
    # [0-9] is ASCII only. \d also matches other decimal digits, such as ٢.
    s = re.escape(sep)
    return re.compile(rf"(?<![0-9])(?P<year>[0-9]{{4}}){s}(?P<month>[0-9]{{2}}){s}(?P<day>[0-9]{{2}})(?![0-9])")


def reformat_dates(text: str, from_sep: str, to_sep: str) -> str:
    """Change each date YYYY<from_sep>MM<from_sep>DD in text to DD<to_sep>MM<to_sep>YYYY."""
    # In a replacement template, a backslash starts an escape, so to_sep doubles each one.
    # \g<day> ends at ">", so a digit after it is not part of the group name.
    sep = to_sep.replace("\\", "\\\\")
    return date_pattern(from_sep).sub(rf"\g<day>{sep}\g<month>{sep}\g<year>", text)


def find_dates(text: str, sep: str) -> list[list[str]]:
    """Return [year, month, day] for each date YYYY<sep>MM<sep>DD in text."""
    return [list(m.groups()) for m in date_pattern(sep).finditer(text)]
