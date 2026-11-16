#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-big-integers/.

# big_power BASE EXP
# Prints BASE to the power EXP as decimal text. -1000 <= BASE <= 1000.
# Bash arithmetic uses fixed-width integers with no overflow check, so the
# function keeps the digits in an array of base-10^9 limbs, least
# significant first.
big_power() {
  local base=$1 exp=$2 m i j t carry out
  local -a limbs=(1)
  m=$(( base < 0 ? -base : base ))
  for (( i = 0; i < exp; i++ )); do
    carry=0
    for (( j = 0; j < ${#limbs[@]}; j++ )); do
      t=$(( limbs[j] * m + carry ))
      limbs[j]=$(( t % 1000000000 ))
      carry=$(( t / 1000000000 ))
    done
    if (( carry > 0 )); then
      limbs+=("$carry")
    fi
  done
  out=""
  if (( base < 0 && exp % 2 == 1 )); then
    out="-"
  fi
  j=$(( ${#limbs[@]} - 1 ))
  out+=${limbs[j]}
  for (( j = j - 1; j >= 0; j-- )); do
    printf -v t '%09d' "${limbs[j]}" # keep the inner zeros
    out+=$t
  done
  printf '%s\n' "$out"
}
