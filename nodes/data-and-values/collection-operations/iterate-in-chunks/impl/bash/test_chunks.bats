#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-in-chunks/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/chunks.sh"
}

check() { # check WANT SIZE ITEM...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(chunks "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "pairs" { check "1 2\n3 4\n5\n" 2 1 2 3 4 5; }
@test "triples" { check "1 2 3\n4 5 6\n" 3 1 2 3 4 5 6; }
@test "empty" { check "" 3; }
@test "size-one" { check "7\n8\n9\n" 1 7 8 9; }
@test "size-larger" { check "1 2\n" 5 1 2; }
@test "size-equal" { check "1 2 3\n" 3 1 2 3; }
@test "last-shorter" { check "1 2 3\n4\n" 3 1 2 3 4; }
@test "negative" { check "-1 -2\n-3\n" 2 -1 -2 -3; }
@test "extremes" { check "-2147483648 2147483647\n0\n" 2 -2147483648 2147483647 0; }
