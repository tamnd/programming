#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

# filter_lines PATH PATTERN
# Prints the lines of the file at PATH that match PATTERN, a POSIX extended
# regular expression, each with an LF at the end. Returns a nonzero status,
# and prints nothing on stdout, if the file cannot be read or the pattern is
# not valid.
filter_lines() {
  [[ -r $1 && ! -d $1 ]] || { printf 'filter_lines: cannot read %s\n' "$1" >&2; return 1; }
  # -e marks the pattern, so a pattern that starts with - is not an option.
  # LC_ALL=C selects the POSIX locale, so the locale of the user does not
  # change the result.
  local status=0
  LC_ALL=C grep -E -e "$2" -- "$1" || status=$?
  # grep returns 1 if no line matched. For this function, that is not an
  # error: the result is no lines.
  ((status <= 1))
}
