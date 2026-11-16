#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/return-multiple-values/.

# div_mod A B
# Prints the quotient and the remainder of the floor division of A by B,
# with a space between them. The caller reads them with read -r.
div_mod() {
  local a=$1 b=$2
  # The operators / and % truncate toward zero. Move a result that is not
  # exact one step down, so that the quotient is the floor.
  local q=$((a / b)) r=$((a % b))
  if ((r != 0 && (r < 0) != (b < 0))); then
    ((q -= 1, r += b))
  fi
  echo "$q $r"
}
