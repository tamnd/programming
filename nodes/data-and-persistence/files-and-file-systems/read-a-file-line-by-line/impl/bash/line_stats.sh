#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-file-line-by-line/.
# Needs Bash 4.4 or later, for "wait" on a process substitution.

# line_stats PATH
# Prints "LINES LONGEST": the number of lines, and the characters in the longest line.
# Returns a nonzero status, and prints nothing on stdout, if the file is missing or not valid UTF-8.
line_stats() {
  local LC_ALL=C.UTF-8 # ${#line} counts characters only in a UTF-8 locale.
  local line lines=0 longest=0
  # IFS= keeps leading and trailing blanks. -r keeps backslashes.
  # read fails on a last line with no newline, but it still sets $line.
  while IFS= read -r line || [[ -n $line ]]; do
    # ${line%$'\r'} also works, but it takes quadratic time on a long line.
    if [[ $line == *$'\r' ]]; then
      line=${line:0:-1}
    fi
    lines=$((lines + 1))
    if ((${#line} > longest)); then
      longest=${#line}
    fi
  done < <(iconv -f UTF-8 -t UTF-8 -- "$1")
  # iconv stops with an error on a bad byte sequence or a missing file.
  wait "$!" || return
  printf '%d %d\n' "$lines" "$longest"
}
