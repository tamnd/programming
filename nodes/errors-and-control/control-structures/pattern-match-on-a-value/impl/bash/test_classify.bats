#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pattern-match-on-a-value/.

setup() {
  source "$BATS_TEST_DIRNAME/classify.sh"
}

check() { # check X Y WANT
  local got
  got=$(classify "$1" "$2")
  [ "$got" = "$3" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$3"; return 1; }
}

@test "x-axis" { check 5 0 x-axis; }
@test "y-axis" { check 0 -3 y-axis; }
@test "diagonal" { check 4 4 diagonal; }
@test "other" { check 2 7 other; }
@test "origin" { check 0 0 origin; }
@test "negative-diagonal" { check -2 -2 diagonal; }
@test "anti-diagonal" { check 3 -3 other; }
@test "negative-x-axis" { check -7 0 x-axis; }
@test "one-one" { check 1 1 diagonal; }
