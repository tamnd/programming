#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-priority-queue/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/serve_order.sh"
}

check() { # check WANT PRIORITY...
  local want=$1
  shift
  run serve_order "$@"
  [ "$status" -eq 0 ]
  [ "$output" = "$want" ] || { printf 'got:  %s\nwant: %s\n' "$output" "$want"; return 1; }
}

@test "example" { check '1 3 0 2' 2 5 1 5; }
@test "longer" { check '5 7 4 2 0 6 1 3' 3 1 4 1 5 9 2 6; }
@test "empty" { check ''; }
@test "one" { check '0' 7; }
@test "ties" { check '0 1 2 3' 4 4 4 4; }
@test "negative" { check '1 2 0' -3 -1 -2; }
@test "int32-extremes" { check '1 2 0 3' -2147483648 2147483647 0 -2147483648; }
@test "sorted" { check '2 1 0' 1 2 3; }
@test "reversed" { check '0 1 2' 3 2 1; }
