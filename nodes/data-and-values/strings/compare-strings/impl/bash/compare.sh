# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-strings/.

# compare A B: prints -1, 0 or 1. The < operator of [[ uses the order of the
# current locale, so the function sets LC_ALL=C in a subshell, which gives the
# order of the bytes.
compare() {
  (
    LC_ALL=C
    if [[ $1 < $2 ]]; then
      printf '%s' -1
    elif [[ $1 == "$2" ]]; then
      printf '%s' 0
    else
      printf '%s' 1
    fi
  )
}

# equals A B: exits with status 0 if A and B are the same string. The quotes
# make B a literal string, not a pattern.
equals() {
  [[ $1 == "$2" ]]
}
