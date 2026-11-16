#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

ROMAN_VALUES=(1000 900 500 400 100 90 50 40 10 9 5 4 1)
ROMAN_SYMBOLS=(M CM D CD C XC L XL X IX V IV I)
declare -gA ROMAN_DIGITS=([I]=1 [V]=5 [X]=10 [L]=50 [C]=100 [D]=500 [M]=1000)

# to_roman N
# Prints the Roman numeral of N. Returns 1 if N is not in 1 to 3999.
to_roman() {
  local n=$1 s="" i
  if ! [[ $n =~ ^[0-9]+$ ]] || ((n < 1 || n > 3999)); then
    return 1
  fi
  for i in "${!ROMAN_VALUES[@]}"; do
    while ((n >= ROMAN_VALUES[i])); do
      s+=${ROMAN_SYMBOLS[i]}
      n=$((n - ROMAN_VALUES[i]))
    done
  done
  printf '%s\n' "$s"
}

# from_roman S
# Prints the value of the Roman numeral S. Returns 1 if S is not a Roman
# numeral in the standard form.
from_roman() {
  local s=$1 total=0 i v next back
  if [[ -z $s || $s == *[!IVXLCDM]* ]]; then
    return 1
  fi
  for ((i = 0; i < ${#s}; i++)); do
    v=${ROMAN_DIGITS[${s:i:1}]}
    next=0
    if ((i + 1 < ${#s})); then
      next=${ROMAN_DIGITS[${s:i+1:1}]}
    fi
    if ((v < next)); then
      total=$((total - v))
    else
      total=$((total + v))
    fi
  done
  back=$(to_roman "$total") || return 1
  [[ $back == "$s" ]] || return 1
  printf '%s\n' "$total"
}
