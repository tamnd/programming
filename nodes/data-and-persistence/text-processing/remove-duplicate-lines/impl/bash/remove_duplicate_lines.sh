#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicate-lines/.

# remove_duplicate_lines PATH
# Prints the lines of the file at PATH, with only the first copy of each line,
# in the order of the file, each with an LF at the end. Returns a nonzero
# status, and prints nothing on stdout, if the file cannot be read.
remove_duplicate_lines() {
  [[ -r $1 && ! -d $1 ]] || { printf 'remove_duplicate_lines: cannot read %s\n' "$1" >&2; return 1; }
  # seen[$0] is 0 the first time that awk sees a line, so !seen[$0]++ is true
  # and awk prints the line. The ++ makes it 1, so the next copies are not
  # printed. LC_ALL=C makes awk compare the bytes of the lines.
  LC_ALL=C awk '!seen[$0]++' "$1"
}
