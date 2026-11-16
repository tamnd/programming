#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-anagrams/.

# _anagram_key text: print the letters of text, in lower case, in sorted order.
_anagram_key() {
  local s=${1,,}
  s=${s//[^a-z]/}
  [[ -z $s ]] && return
  printf '%s' "$s" | fold -w1 | LC_ALL=C sort | tr -d '\n'
}

# are_anagrams a b: print true if a and b hold each letter from a to z the
# same number of times, and false if not. Case, spaces, punctuation and
# digits do not count.
are_anagrams() {
  if [[ $(_anagram_key "$1") == "$(_anagram_key "$2")" ]]; then
    echo true
  else
    echo false
  fi
}
