#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/round-a-number/.

# round_half_away X
# Prints the decimal text X rounded to the nearest integer; a half goes away
# from zero. Bash arithmetic has no floats, so the function reads the text:
# the integer part, then the first digit after the point.
round_half_away() {
  local x=$1 sign='' int frac=''
  if [[ $x == -* ]]; then
    sign=-
    x=${x#-}
  fi
  int=${x%%.*}
  [[ $x == *.* ]] && frac=${x#*.}
  # 10# reads the digits as decimal, so 010 is 10 and not octal 8.
  int=$((10#${int:-0}))
  if [[ ${frac:0:1} == [5-9] ]]; then
    int=$((int + 1))
  fi
  if ((int == 0)); then
    sign=''
  fi
  echo "$sign$int"
}
