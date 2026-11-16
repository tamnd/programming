#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-an-enumeration/.

# Bash has no enum. An indexed array lists the names in order, and the index
# of a name is its integer value.
DAYS=(monday tuesday wednesday thursday friday saturday sunday)

# cycle START STEPS
# Prints STEPS day names, one on each line, from START on, with monday after
# sunday. Prints nothing if START is not a name.
cycle() {
  local start=$1 steps=$2 i k
  for i in "${!DAYS[@]}"; do
    if [[ ${DAYS[i]} == "$start" ]]; then
      for ((k = 0; k < steps; k++)); do
        printf '%s\n' "${DAYS[(i + k) % ${#DAYS[@]}]}"
      done
      return 0
    fi
  done
}
