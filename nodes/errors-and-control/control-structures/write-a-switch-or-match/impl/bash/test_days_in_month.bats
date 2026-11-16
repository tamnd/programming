#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-switch-or-match/.

setup() {
  source "$BATS_TEST_DIRNAME/days_in_month.sh"
}

check() { # check MONTH WANT
  local got
  got=$(days_in_month "$1")
  [ "$got" = "$2" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$2"; return 1; }
}

@test "january" { check 1 31; }
@test "february" { check 2 28; }
@test "april" { check 4 30; }
@test "july" { check 7 31; }
@test "november" { check 11 30; }
@test "december" { check 12 31; }
@test "zero" { check 0 0; }
@test "thirteen" { check 13 0; }
@test "minus-one" { check -1 0; }
