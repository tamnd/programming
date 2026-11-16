#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-an-enum-with-data/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/perimeter.sh"
}

check() { # check WANT KIND SIDE...; WANT is none for no result
  local want=$1 got status=0
  shift
  got=$(perimeter "$@") || status=$?
  if [ "$want" = none ]; then
    [ "$status" -ne 0 ] && [ -z "$got" ] || { printf 'got: %s\n' "$got"; return 1; }
  else
    [ "$status" -eq 0 ] && [ "$got" = "$want" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$want"; return 1; }
  fi
}

@test "square" { check 12 square 3; }
@test "rectangle" { check 14 rectangle 2 5; }
@test "triangle" { check 12 triangle 3 4 5; }
@test "unknown-kind" { check none circle 2; }
@test "wrong-case" { check none Square 3; }
@test "too-few-sides" { check none rectangle 4; }
@test "too-many-sides" { check none square 3 3; }
@test "no-sides" { check none triangle; }
@test "zero-side" { check none square 0; }
@test "negative-side" { check none rectangle -2 5; }
@test "not-a-triangle" { check none triangle 1 2 3; }
@test "largest" { check 3000000 triangle 1000000 1000000 1000000; }
@test "variants carry their data" { [ "$(make_shape rectangle 2 5)" = "rectangle 2 5" ]; }
