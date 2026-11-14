#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-set-operations/.

# compare_sets A_WORD... -- B_WORD...
# Prints five lines: the union, the intersection, the difference and the
# symmetric difference of the two sets, each in ascending order by bytes with
# the words split by spaces, and then true if the set of A is a subset of the
# set of B, else false.
compare_sets() {
  # Bash has no set type. The keys of an associative array are the set.
  local -A in_a=() in_b=()
  local word
  while (($# > 0)) && [[ $1 != -- ]]; do
    in_a[$1]=1
    shift
  done
  shift # the --
  for word in "$@"; do
    in_b[$word]=1
  done
  local -a either=() both=() only_a=() one=()
  local subset=true
  for word in "${!in_a[@]}"; do
    either+=("$word")
    if [[ -v in_b[$word] ]]; then
      both+=("$word")
    else
      only_a+=("$word")
      one+=("$word")
      subset=false
    fi
  done
  for word in "${!in_b[@]}"; do
    if [[ ! -v in_a[$word] ]]; then
      either+=("$word")
      one+=("$word")
    fi
  done
  _sorted_line "${either[@]}"
  _sorted_line "${both[@]}"
  _sorted_line "${only_a[@]}"
  _sorted_line "${one[@]}"
  echo "$subset"
}

# _sorted_line WORD...
# Prints the words in ascending order by bytes on one line.
_sorted_line() {
  if (($# == 0)); then
    echo
    return
  fi
  printf '%s\n' "$@" | LC_ALL=C sort | paste -sd ' ' -
}
