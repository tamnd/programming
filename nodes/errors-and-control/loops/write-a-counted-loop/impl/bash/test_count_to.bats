#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-counted-loop/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/count_to.sh"
}

check() { # check N -- WANT...
  local n=$1
  shift 2
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(count_to "$n"; printf x)
  want=$(if (($# > 0)); then printf '%s\n' "$@"; fi; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "ten" { check 10 -- 1 2 3 4 5 6 7 8 9 10; }
@test "three" { check 3 -- 1 2 3; }
@test "zero" { check 0 --; }
@test "one" { check 1 -- 1; }
@test "two" { check 2 -- 1 2; }
