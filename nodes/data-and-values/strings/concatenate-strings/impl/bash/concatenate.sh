# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-strings/.

# concat A B: prints A followed by B. Two expansions side by side make one
# word, so "$1$2" is the concatenation.
concat() {
  printf '%s' "$1$2"
}

# concat_number S N: prints S followed by the decimal form of the integer N.
# A Bash value is a string, so a number needs no conversion. %d checks that N
# is an integer.
concat_number() {
  printf '%s%d' "$1" "$2"
}
