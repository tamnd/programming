#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-list/.

# reverse_list ITEM...
# Prints the items in the opposite order, one on each line.
reverse_list() {
  local i
  # The positional parameters have no gaps, so $# is the last position.
  for ((i = $#; i >= 1; i--)); do
    printf '%s\n' "${!i}"
  done
}
