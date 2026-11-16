#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/print-formatted-output/.

# format_row name qty price: print one line of a table, with name in 10
# columns on the left, qty in 5 on the right and price with 2 decimals in 8
# on the right. LC_NUMERIC=C keeps the decimal point a dot.
format_row() {
  local line
  LC_NUMERIC=C printf -v line '%-10s|%5d|%8.2f' "$1" "$2" "$3"
  printf '%s\n' "$line"
}
