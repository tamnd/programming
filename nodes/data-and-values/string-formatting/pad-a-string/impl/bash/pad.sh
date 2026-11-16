#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pad-a-string/.

# _pad_extra width s fill: set the variable extra to fill repeated
# max(0, width - length of s) times.
_pad_extra() {
  local n=$(($1 - ${#2}))
  extra=''
  if ((n > 0)); then
    printf -v extra '%*s' "$n" ''
    extra=${extra// /"$3"}
  fi
}

# pad_left s width fill: print s padded with fill on the left to width.
pad_left() {
  local extra
  _pad_extra "$2" "$1" "$3"
  printf '%s\n' "$extra$1"
}

# pad_right s width fill: print s padded with fill on the right to width.
pad_right() {
  local extra
  _pad_extra "$2" "$1" "$3"
  printf '%s\n' "$1$extra"
}
