#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/zip_join.sh"
}

check() { # check WANT... (the arrays are lower, upper and digits)
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(zip_join lower upper digits; printf x)
  want=$(if (($# > 0)); then printf '%s\n' "$@"; fi; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "rosetta" {
  lower=(a b c) upper=(A B C) digits=(1 2 3)
  check aA1 bB2 cC3
}
@test "words" {
  lower=(x y) upper=(X Y) digits=(10 20)
  check xX10 yY20
}
@test "shorter-first" {
  lower=(a) upper=(A B) digits=(1 2 3)
  check aA1
}
@test "shorter-last" {
  lower=(a b c) upper=(A B C) digits=(1)
  check aA1
}
@test "one-empty" {
  lower=(a b) upper=() digits=(1 2)
  check
}
@test "all-empty" {
  lower=() upper=() digits=()
  check
}
@test "negative" {
  lower=(a) upper=(A) digits=(-5)
  check aA-5
}
@test "empty-strings" {
  lower=("" b) upper=(A "") digits=(1 2)
  check A1 b2
}
