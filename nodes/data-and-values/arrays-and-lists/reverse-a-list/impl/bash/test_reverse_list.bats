#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/reverse_list.sh"
}

lines() { # lines ITEM...: prints each item on a line, and nothing for no items
  if (($# > 0)); then printf '%s\n' "$@"; fi
}

check() { # check "ITEMS" -- WANT...
  local -a items
  read -r -a items <<<"$1"
  shift 2
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(reverse_list "${items[@]}"; printf x)
  want=$(lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "three" { check "1 2 3" -- 3 2 1; }
@test "four" { check "4 7 1 9" -- 9 1 7 4; }
@test "empty" { check "" --; }
@test "single" { check "5" -- 5; }
@test "two" { check "1 2" -- 2 1; }
@test "duplicates" { check "1 1 2" -- 2 1 1; }
@test "palindrome" { check "1 2 1" -- 1 2 1; }
@test "negative" { check "-1 0 -3" -- -3 0 -1; }
@test "limit" { check "2147483647 -2147483648 0" -- 0 -2147483648 2147483647; }
@test "spaces" { got=$(reverse_list a "New York" "" Paris; printf x); [ "$got" = $'Paris\n\nNew York\na\nx' ]; }
