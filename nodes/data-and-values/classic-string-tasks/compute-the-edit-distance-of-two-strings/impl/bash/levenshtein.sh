#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

# levenshtein a b: print the minimum number of one-character insertions,
# deletions and substitutions that change a into b. Two rows of the table
# are kept. Each step runs in the shell, so use it for short words only.
levenshtein() {
  local a=$1 b=$2 i j cost best
  local -a prev cur
  for ((j = 0; j <= ${#b}; j++)); do
    prev[j]=$j
  done
  for ((i = 1; i <= ${#a}; i++)); do
    cur=("$i")
    for ((j = 1; j <= ${#b}; j++)); do
      cost=1
      [[ ${a:i-1:1} == "${b:j-1:1}" ]] && cost=0
      best=$((prev[j - 1] + cost))
      ((prev[j] + 1 < best)) && best=$((prev[j] + 1))
      ((cur[j - 1] + 1 < best)) && best=$((cur[j - 1] + 1))
      cur[j]=$best
    done
    prev=("${cur[@]}")
  done
  echo "${prev[${#b}]}"
}
