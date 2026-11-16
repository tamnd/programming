#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-tuple/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/min_max.sh"
}

check() { # check LO HI NUMBER...
  local lo hi want_lo=$1 want_hi=$2
  shift 2
  read -r lo hi < <(min_max "$@")
  [ "$lo $hi" = "$want_lo $want_hi" ] || { printf 'got:  %s %s\nwant: %s %s\n' "$lo" "$hi" "$want_lo" "$want_hi"; return 1; }
}

@test "mixed" { check 1 3 3 1 2; }
@test "longer" { check -2 9 4 9 -2 7 0; }
@test "one" { check 5 5 5; }
@test "negative" { check -7 -1 -3 -1 -7; }
@test "all-same" { check 2 2 2 2 2; }
@test "int32-extremes" { check -2147483648 2147483647 2147483647 0 -2147483648; }
@test "sorted" { check 1 4 1 2 3 4; }
@test "reversed" { check 1 4 4 3 2 1; }
