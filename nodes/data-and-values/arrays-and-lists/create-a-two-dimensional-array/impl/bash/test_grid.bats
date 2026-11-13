#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/grid.sh"
}

lines() { # lines ROW...: prints each row on a line, and nothing for no rows
  if (($# > 0)); then printf '%s\n' "$@"; fi
}

check() { # check ROWS COLS VALUE -- ROW...
  local rows=$1 cols=$2 value=$3
  shift 4
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(grid "$rows" "$cols" "$value"; printf x)
  want=$(lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "two-by-three" { check 2 3 0 -- "0 0 0" "0 0 0"; }
@test "three-by-one" { check 3 1 7 -- "7" "7" "7"; }
@test "no-rows" { check 0 3 1 --; }
@test "no-cols" { check 2 0 1 -- "" ""; }
@test "one-cell" { check 1 1 5 -- "5"; }
@test "one-row" { check 1 4 2 -- "2 2 2 2"; }
@test "one-col" { check 4 1 2 -- "2" "2" "2" "2"; }
@test "negative" { check 2 2 -3 -- "-3 -3" "-3 -3"; }
@test "min-int" { check 2 2 -2147483648 -- "-2147483648 -2147483648" "-2147483648 -2147483648"; }
@test "max-int" { check 1 2 2147483647 -- "2147483647 2147483647"; }
