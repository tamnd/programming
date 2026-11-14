#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

# count_lines PATH
# Prints "NEWLINES LINES": the number of LF bytes, as wc -l counts them, and the
# number of lines, which also counts a last line that has no LF.
# Returns a nonzero status, and prints nothing on stdout, if the file cannot be read.
count_lines() {
  local newlines last
  # With the file on stdin, wc prints no file name.
  newlines=$(wc -l <"$1") || return
  # The last byte of the file: 1 if it is an LF, 0 if it is not or the file is empty.
  last=$(tail -c 1 -- "$1" | wc -l) || return
  # $((...)) also accepts blanks around a number.
  newlines=$((newlines))
  if [[ -s $1 ]] && ((last == 0)); then
    printf '%d %d\n' "$newlines" "$((newlines + 1))"
  else
    printf '%d %d\n' "$newlines" "$newlines"
  fi
}
