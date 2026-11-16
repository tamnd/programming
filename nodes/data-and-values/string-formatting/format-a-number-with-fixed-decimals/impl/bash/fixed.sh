#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

# fixed x digits: print x with exactly digits digits after the decimal point.
# The shell printf reads x as a long double, so the extra digits are not
# those of a double. awk uses a double, and LC_ALL=C gives a dot.
fixed() {
  LC_ALL=C awk -v x="$1" -v d="$2" 'BEGIN { printf("%." d "f\n", x) }'
}
