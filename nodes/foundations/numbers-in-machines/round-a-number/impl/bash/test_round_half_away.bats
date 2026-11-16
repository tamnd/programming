#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/round-a-number/.

setup() {
  source "$BATS_TEST_DIRNAME/round_half_away.sh"
}

check() { # check WANT X
  local got
  got=$(round_half_away "$2")
  [ "$got" = "$1" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$1"; return 1; }
}

@test "down" { check 2 2.4; }
@test "up" { check 3 2.6; }
@test "negative" { check -3 -2.6; }
@test "integer" { check 7 7.0; }
@test "half-even-up" { check 3 2.5; }
@test "half-odd-up" { check 2 1.5; }
@test "half-zero" { check 1 0.5; }
@test "half-negative" { check -3 -2.5; }
@test "half-negative-small" { check -1 -0.5; }
@test "negative-below-half" { check 0 -0.4; }
@test "near-half" { check 0 0.49999999999999994; }
@test "zero" { check 0 0.0; }
@test "large-odd" { check 4503599627370497 4503599627370497.0; }
@test "large-half-negative" { check -1000000000000001 -1000000000000000.5; }
@test "printf rounds a half to even" {
  [ "$(LC_ALL=C printf '%.0f %.0f' 2.5 3.5)" = "2 4" ]
}
