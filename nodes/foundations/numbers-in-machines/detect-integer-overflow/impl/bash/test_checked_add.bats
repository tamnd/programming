#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-integer-overflow/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/checked_add.sh"
}

check() { # check WANT A B; WANT is none for no result
  local want=$1 got status=0
  shift
  got=$(checked_add "$@") || status=$?
  if [ "$want" = none ]; then
    [ "$status" -ne 0 ] && [ -z "$got" ] || { printf 'got: %s\n' "$got"; return 1; }
  else
    [ "$status" -eq 0 ] && [ "$got" = "$want" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$want"; return 1; }
  fi
}

@test "small" { check 5 2 3; }
@test "negative" { check -3 -7 4; }
@test "beyond-32-bit" { check 9000000000 4000000000 5000000000; }
@test "max-plus-zero" { check 9223372036854775807 9223372036854775807 0; }
@test "max-plus-one" { check none 9223372036854775807 1; }
@test "max-plus-max" { check none 9223372036854775807 9223372036854775807; }
@test "min-plus-zero" { check -9223372036854775808 -9223372036854775808 0; }
@test "min-minus-one" { check none -9223372036854775808 -1; }
@test "min-plus-min" { check none -9223372036854775808 -9223372036854775808; }
@test "min-plus-max" { check -1 -9223372036854775808 9223372036854775807; }
@test "max-minus-one" { check 9223372036854775806 9223372036854775807 -1; }
@test "plus wraps" { [ "$((INT64_MAX + 1))" = "$INT64_MIN" ]; }
