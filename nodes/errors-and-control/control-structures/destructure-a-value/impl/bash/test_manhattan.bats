#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/destructure-a-value/.

setup() {
  source "$BATS_TEST_DIRNAME/manhattan.sh"
}

check() { # check WANT (the points are the arrays p and q)
  local got
  got=$(manhattan p q)
  [ "$got" = "$1" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$1"; return 1; }
}

@test "first" {
  p=(1 2)
  q=(4 6)
  check 7
}

@test "on-a-line" {
  p=(0 0)
  q=(10 0)
  check 10
}

@test "down-left" {
  p=(5 5)
  q=(2 1)
  check 7
}

@test "mixed-signs" {
  p=(3 -1)
  q=(-2 4)
  check 10
}

@test "same-point" {
  p=(7 7)
  q=(7 7)
  check 0
}

@test "negative" {
  p=(-3 -4)
  q=(-1 -1)
  check 5
}

@test "large" {
  p=(1000000 -1000000)
  q=(-1000000 1000000)
  check 4000000
}
