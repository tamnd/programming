#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

# read_line PATH NUMBER
# Prints line NUMBER of the file at PATH with an LF after it. Line 1 is the
# first line. Returns 0 if the line is there, 1 if there is no such line, and
# 2 if the file cannot be read.
read_line() {
  [[ -r $1 && ! -d $1 ]] || { printf 'read_line: cannot read %s\n' "$1" >&2; return 2; }
  # NR is the number of the current line. exit stops the read at the line,
  # and the END action gives the status. The file comes on standard input,
  # so awk does not take a name such as x=1 as an assignment.
  awk -v n="$2" 'NR == n { print; found = 1; exit } END { exit !found }' <"$1"
}
