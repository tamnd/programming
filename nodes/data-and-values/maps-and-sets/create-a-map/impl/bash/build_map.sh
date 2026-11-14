#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-map/.

# build_map KEY=VALUE... [-- KEY...]
# Puts each KEY=VALUE pair in an associative array, so a later pair replaces
# the value of the key, then unsets each KEY after --. Prints one line
# "KEY VALUE" for each entry, in ascending order of the key by its bytes.
build_map() {
  local -A map=()
  local key
  while (($# > 0)) && [[ $1 != -- ]]; do
    map[${1%%=*}]=${1#*=}
    shift
  done
  if (($# > 0)); then
    shift
  fi
  for key in "$@"; do
    # The quotes stop the shell from expanding the subscript as a glob.
    unset 'map[$key]'
  done
  # LC_ALL=C makes sort compare bytes, so B comes before a.
  for key in "${!map[@]}"; do
    printf '%s %s\n' "$key" "${map[$key]}"
  done | LC_ALL=C sort
}
