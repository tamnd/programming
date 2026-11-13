#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/contains.sh"
}

check() { # check VALUE "ITEMS" WANT: WANT is yes or no
  local -a items
  read -r -a items <<<"$2"
  if contains "$1" "${items[@]}"; then
    [ "$3" = yes ]
  else
    [ "$3" = no ]
  fi
}

@test "present" { check 2 "1 2 3" yes; }
@test "absent" { check 4 "1 2 3" no; }
@test "empty" { check 0 "" no; }
@test "single-yes" { check 5 "5" yes; }
@test "single-no" { check 0 "5" no; }
@test "first" { check 7 "7 8 9" yes; }
@test "last" { check 9 "7 8 9" yes; }
@test "duplicates" { check 2 "2 2 2" yes; }
@test "negative" { check -2 "-1 -2 -3" yes; }
@test "zero-absent" { check 0 "-1 1" no; }
@test "limit" { check -2147483648 "2147483647 -2147483648" yes; }
@test "limit-absent" { check -2147483648 "2147483647" no; }
@test "pattern" { if contains "*" a b; then return 1; fi; contains "*" a "*"; }
@test "spaces" { if contains York "New York" Paris; then return 1; fi; contains "New York" "New York" Paris; }
