#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

# first_and_last_lines PATH COUNT FIRST LAST
# Puts the first COUNT lines of the file at PATH into the array named FIRST,
# and the last COUNT lines into the array named LAST, with no LF at the end of
# each line. Returns a nonzero status if the file cannot be read. Needs bash
# 4.3 or later, for local -n.
first_and_last_lines() {
  [[ -r $1 && ! -d $1 ]] || { printf 'first_and_last_lines: cannot read %s\n' "$1" >&2; return 1; }
  local -n first_and_last_lines_first=$3 first_and_last_lines_last=$4
  first_and_last_lines_first=()
  first_and_last_lines_last=()
  # POSIX lets head refuse -n 0, and some versions do. So a count of 0 does
  # not run head or tail.
  (($2 > 0)) || return 0
  # mapfile -t removes the LF at the end of each line, but keeps a CR.
  mapfile -t first_and_last_lines_first < <(head -n "$2" -- "$1")
  mapfile -t first_and_last_lines_last < <(tail -n "$2" -- "$1")
}
