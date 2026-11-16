#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

# describe_order name count price: print "Order {name}: count item(s), total
# count*price". Bash has no float arithmetic, so awk computes the total.
describe_order() {
  local name=$1 count=$2 price=$3 ending=s total
  if ((count == 1)); then
    ending=
  fi
  total=$(LC_ALL=C awk -v c="$count" -v p="$price" 'BEGIN { printf "%.2f", c * p }')
  printf '%s\n' "Order {${name}}: ${count} item${ending}, total ${total}"
}
