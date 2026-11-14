#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

# sort_lines PATH
# Prints the lines of the file at PATH, sorted by their bytes, each with an LF
# at the end. For UTF-8 text, the order of the bytes is the order of the code
# points. Returns a nonzero status, and prints nothing on stdout, if the file
# cannot be read.
sort_lines() {
  [[ -r $1 && ! -d $1 ]] || { printf 'sort_lines: cannot read %s\n' "$1" >&2; return 1; }
  # In the POSIX locale, sort compares the bytes. In another locale, it uses
  # the collation of that locale, so the order changes from machine to
  # machine. sort adds an LF to a last line that has none.
  LC_ALL=C sort -- "$1"
}
