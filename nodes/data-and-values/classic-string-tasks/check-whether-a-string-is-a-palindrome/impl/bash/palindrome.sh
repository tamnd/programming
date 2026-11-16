#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

# is_palindrome text: print true if the ASCII letters and digits of text, in
# lower case, read the same backwards, and false if not.
is_palindrome() {
  local s=${1,,} i n
  s=${s//[^a-z0-9]/}
  n=${#s}
  for ((i = 0; i < n / 2; i++)); do
    if [[ ${s:i:1} != "${s:n-1-i:1}" ]]; then
      echo false
      return
    fi
  done
  echo true
}
