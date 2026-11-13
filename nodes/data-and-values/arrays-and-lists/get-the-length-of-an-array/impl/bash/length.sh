#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-length-of-an-array/.

# length ITEM...
# Prints the number of arguments. To count an array, call it as
# length "${a[@]}", or use ${#a[@]} directly.
length() {
  printf '%s\n' "$#"
}
