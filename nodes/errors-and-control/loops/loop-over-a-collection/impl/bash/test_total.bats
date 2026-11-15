#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-a-collection/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/total.sh"
}

check() { # check WANT ITEM...
  local want=$1
  shift
  local got
  got=$(total "$@"; printf x)
  [ "$got" = "$want"$'\n'x ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "three" { check 6 1 2 3; }
@test "mixed" { check 16 10 -4 7 0 3; }
@test "empty" { check 0; }
@test "single" { check 7 7; }
@test "negative" { check -4 -5 2 -1; }
@test "limit" { check 4294967296 2147483647 2147483647 2; }
@test "limit-low" { check -4294967296 -2147483648 -2147483648; }
