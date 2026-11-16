#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-integer-arithmetic/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/arith.sh"
}

check() { # check WANT A B N
  local want=$1 got
  shift
  got=$(arith "$@")
  [ "$got" = "$want" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$want"; return 1; }
}

@test "small" { check "9 5 14 3 1 343" 7 2 3; }
@test "exact-division" { check "25 15 100 4 0 400" 20 5 2; }
@test "negative-dividend" { check "-5 -9 -14 -3 -1 -343" -7 2 3; }
@test "negative-divisor" { check "5 9 -14 -3 1 49" 7 -2 2; }
@test "both-negative" { check "-9 -5 14 3 -1 -7" -7 -2 1; }
@test "zero-dividend" { check "5 -5 0 0 0 1" 0 5 0; }
@test "zero-power" { check "8 2 15 1 2 1" 5 3 0; }
@test "divisor-larger" { check "10 -4 21 0 3 3" 3 7 1; }
@test "largest" { check "200000 0 10000000000 1 0 1000000000000000" 100000 100000 3; }
@test "smallest" { check "-99999 -100001 -100000 -100000 0 -1000000000000000" -100000 1 3; }
@test "caret is not a power" { [ "$((2 ^ 3))" = 1 ]; }
