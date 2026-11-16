#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-function/.

# clamp VALUE LOW HIGH
# Prints VALUE limited to the range from LOW to HIGH. A Bash function
# returns only an exit status from 0 to 255, so the result goes to the
# standard output, and the caller reads it with $( ).
clamp() {
  local value=$1 low=$2 high=$3
  if ((value < low)); then
    echo "$low"
  elif ((value > high)); then
    echo "$high"
  else
    echo "$value"
  fi
}
