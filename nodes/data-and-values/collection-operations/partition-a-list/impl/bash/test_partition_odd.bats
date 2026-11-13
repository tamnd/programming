#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/partition-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/partition_odd.sh"
}

check() { # check ODD_LINE EVEN_LINE GIVEN...
  local want_odd=$1 want_even=$2 got want
  shift 2
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(partition_odd "$@"; printf x)
  want=$(printf '%s\n' "$want_odd" "$want_even"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "mixed" { check "1 3 5" "2 4" 1 2 3 4 5; }
@test "tens" { check "15 25" "10 20" 10 15 20 25; }
@test "empty" { check "" ""; }
@test "all-odd" { check "1 3" "" 1 3; }
@test "all-even" { check "" "2 4" 2 4; }
@test "negative" { check "-3 -1" "-2 0" -3 -2 -1 0; }
@test "zero" { check "" "0" 0; }
@test "order" { check "9 1" "8 2" 9 8 1 2; }
@test "extremes" { check "2147483647" "-2147483648" -2147483648 2147483647; }
@test "duplicates" { check "3 3" "2 2" 3 3 2 2; }
