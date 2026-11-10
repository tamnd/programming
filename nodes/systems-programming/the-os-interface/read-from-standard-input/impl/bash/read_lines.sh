#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-from-standard-input/.
# Runs on Bash 3.2 and later.

# read_lines
# Reads standard input to its end and puts its lines in the global array "lines", each without its "\n" or "\r\n".
read_lines() {
  lines=()
  local line
  # IFS= keeps leading and trailing blanks. -r keeps backslashes.
  # read fails at the end of input. On a last line with no newline it fails too, but it still sets $line.
  while IFS= read -r line || [[ -n $line ]]; do
    if [[ $line == *$'\r' ]]; then
      line=${line%?}
    fi
    lines+=("$line")
  done
}

# Run as a script, it prints each line.
if [[ ${BASH_SOURCE[0]} == "$0" ]]; then
  read_lines
  if ((${#lines[@]} > 0)); then
    printf '%s\n' "${lines[@]}"
  fi
fi
