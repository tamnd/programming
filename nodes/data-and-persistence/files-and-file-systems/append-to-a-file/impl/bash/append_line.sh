#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-file/.

# append_line PATH LINE
# Adds LINE and one newline to the end of the file PATH, and makes the file if it is missing.
# Returns a nonzero status, and writes nothing, if LINE holds a newline or a CR,
# or if the file cannot be opened.
append_line() {
  if [[ $2 == *[$'\r\n']* ]]; then
    printf 'append_line: the line must not hold a newline or a CR\n' >&2
    return 1
  fi
  # >> opens the file with O_APPEND. printf sends the line and the newline together.
  printf '%s\n' "$2" >>"$1"
}
