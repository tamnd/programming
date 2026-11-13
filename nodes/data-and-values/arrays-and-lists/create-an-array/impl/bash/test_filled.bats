#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-an-array/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/filled.sh"
}

lines() { # lines ITEM...: prints each item on a line, and nothing for no items
  if (($# > 0)); then printf '%s\n' "$@"; fi
}

check() { # check N VALUE -- WANT...
  local n=$1 value=$2 got want
  shift 3
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(filled "$n" "$value"; printf x)
  want=$(lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "three-sevens" { check 3 7 -- 7 7 7; }
@test "five-zeros" { check 5 0 -- 0 0 0 0 0; }
@test "empty" { check 0 7 --; }
@test "single" { check 1 42 -- 42; }
@test "zero" { check 2 0 -- 0 0; }
@test "negative" { check 2 -1 -- -1 -1; }
@test "max-int" { check 2 2147483647 -- 2147483647 2147483647; }
@test "min-int" { check 2 -2147483648 -- -2147483648 -2147483648; }
@test "ten" { check 10 1 -- 1 1 1 1 1 1 1 1 1 1; }
