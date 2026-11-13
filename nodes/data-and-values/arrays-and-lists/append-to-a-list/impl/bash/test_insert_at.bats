#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/insert_at.sh"
}

check() { # check "ITEMS" INDEX VALUE -- WANT...
  local -a items
  read -r -a items <<<"$1"
  local index=$2 value=$3
  shift 4
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(insert_at items "$index" "$value"; printf x)
  want=$(printf '%s\n' "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "at-end" { check "1 2" 2 3 -- 1 2 3; }
@test "at-front" { check "2 3" 0 1 -- 1 2 3; }
@test "in-middle" { check "1 3" 1 2 -- 1 2 3; }
@test "empty" { check "" 0 7 -- 7; }
@test "single-front" { check "5" 0 4 -- 4 5; }
@test "single-end" { check "5" 1 6 -- 5 6; }
@test "end-of-many" { check "1 2 3 4" 4 5 -- 1 2 3 4 5; }
@test "front-of-many" { check "1 2 3 4" 0 0 -- 0 1 2 3 4; }
@test "duplicates" { check "1 1" 1 1 -- 1 1 1; }
@test "limit" { check "2147483647" 1 -2147483648 -- 2147483647 -2147483648; }
@test "spaces" { local -a x=("New York" Rome); got=$(insert_at x 1 Paris; printf x); [ "$got" = $'New York\nParis\nRome\nx' ]; }
@test "past-the-end" { local -a x=(1 2); if insert_at x 3 9 >/dev/null; then return 1; fi; }
