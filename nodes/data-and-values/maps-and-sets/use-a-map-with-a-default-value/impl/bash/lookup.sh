#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

# lookup FALLBACK KEY=VALUE... [-- KEY...]
# The pairs before -- are the map, and the words after it are the keys to
# read. Prints one line for each key: the value of the key in the map, or
# FALLBACK if the map has no such key.
lookup() {
  local fallback=$1
  shift
  local -A table=()
  while (($# > 0)) && [[ $1 != -- ]]; do
    table[${1%%=*}]=${1#*=}
    shift
  done
  if (($# > 0)); then
    shift
  fi
  local key
  for key in "$@"; do
    # ${name-word} uses word only if name is unset. ${name:-word} would also
    # use it for an empty value.
    printf '%s\n' "${table[$key]-$fallback}"
  done
}
