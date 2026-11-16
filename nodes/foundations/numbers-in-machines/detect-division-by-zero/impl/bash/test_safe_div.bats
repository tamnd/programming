#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-division-by-zero/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/safe_div.sh"
}

check() { # check WANT A B; WANT is none for no result
  local want=$1 got status=0
  shift
  got=$(safe_div "$@") || status=$?
  if [ "$want" = none ]; then
    [ "$status" -ne 0 ] && [ -z "$got" ] || { printf 'got: %s\n' "$got"; return 1; }
  else
    [ "$status" -eq 0 ] && [ "$got" = "$want" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$want"; return 1; }
  fi
}

@test "exact" { check 3 12 4; }
@test "remainder-dropped" { check 3 7 2; }
@test "zero-dividend" { check 0 0 5; }
@test "zero-divisor" { check none 5 0; }
@test "negative-by-zero" { check none -5 0; }
@test "zero-by-zero" { check none 0 0; }
@test "negative-dividend" { check -3 -7 2; }
@test "negative-divisor" { check -3 7 -2; }
@test "both-negative" { check 3 -7 -2; }
@test "min-by-minus-one" { check none -9223372036854775808 -1; }
@test "min-by-one" { check -9223372036854775808 -9223372036854775808 1; }
@test "max-by-minus-one" { check -9223372036854775807 9223372036854775807 -1; }
@test "min-by-max" { check -1 -9223372036854775808 9223372036854775807; }
@test "plain division by zero is an error" {
  run -1 bash -c 'echo $((5 / 0))'
  [[ "$output" == *"division by 0"* ]]
}
