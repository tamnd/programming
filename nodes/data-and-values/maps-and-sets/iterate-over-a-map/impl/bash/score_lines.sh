#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-a-map/.

# score_lines KEY=VALUE...
# Puts the pairs in an associative array, then prints one line KEY=VALUE for
# each entry in ascending order of the key by its bytes, then total=N with the
# sum of the values.
score_lines() {
  local -A map=()
  local arg key total=0
  for arg in "$@"; do
    map[${arg%%=*}]=${arg#*=}
  done
  # "${!map[@]}" gives the keys, one word each, in no set order.
  for key in "${!map[@]}"; do
    printf '%s=%s\n' "$key" "${map[$key]}"
  done | LC_ALL=C sort
  # "${map[@]}" gives the values.
  for arg in "${map[@]}"; do
    total=$((total + arg))
  done
  printf 'total=%d\n' "$total"
}
