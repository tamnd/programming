#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-with-a-step/.

# count_by START STOP STEP
# Prints START, START + STEP, ... while the value has not passed STOP, one on
# each line. STEP must not be 0.
count_by() {
  local i
  # The test depends on the direction of the step. {$1..$2..$3} would not
  # work, because brace expansion runs before the variables expand.
  for ((i = $1; $3 > 0 ? i <= $2 : i >= $2; i += $3)); do
    printf '%s\n' "$i"
  done
}
