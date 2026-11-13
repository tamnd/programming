#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-with-a-custom-key/.

# sort_by_length WORD...
# Prints the words, one on each line, in order of the length, then of the bytes
# of the word.
sort_by_length() {
  local w
  for w in "$@"; do
    printf '%s %s\n' "${#w}" "$w"
  done | LC_ALL=C sort -k1,1n -k2,2 | cut -d' ' -f2-
}
