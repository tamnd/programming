#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-conditional/.

setup() {
  source "$BATS_TEST_DIRNAME/grade.sh"
}

check() { # check SCORE WANT
  local got
  got=$(grade "$1")
  [ "$got" = "$2" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$2"; return 1; }
}

@test "ninety-five" { check 95 A; }
@test "eighty-five" { check 85 B; }
@test "seventy-five" { check 75 C; }
@test "sixty-five" { check 65 D; }
@test "forty" { check 40 F; }
@test "hundred" { check 100 A; }
@test "zero" { check 0 F; }
@test "ninety" { check 90 A; }
@test "eighty-nine" { check 89 B; }
@test "sixty" { check 60 D; }
@test "fifty-nine" { check 59 F; }
