#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-string/.

# reverse_code_points STRING
# Prints the characters of STRING in the opposite order. In a UTF-8 locale a character is one code point.
# Returns a nonzero status, and prints nothing on stdout, if STRING is not valid UTF-8.
reverse_code_points() {
  local LC_ALL=C.UTF-8 # ${#s} and ${s:i:1} count characters only in a UTF-8 locale.
  local s=$1 out='' i
  # iconv stops with an error on a bad byte sequence.
  printf '%s' "$s" | iconv -f UTF-8 -t UTF-8 >/dev/null || return
  for ((i = ${#s} - 1; i >= 0; i--)); do
    out+=${s:i:1}
  done
  printf '%s\n' "$out"
}
