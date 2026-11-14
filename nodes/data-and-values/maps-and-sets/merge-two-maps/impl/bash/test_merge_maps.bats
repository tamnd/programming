#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/merge-two-maps/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/merge_maps.sh"
}

check() { # check WANT ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(merge_maps "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "prices" { check "apple 3\nfig 2\npear 6\n" apple=3 pear=5 -- pear=6 fig=2; }
@test "disjoint" { check "a 1\nb 2\n" a=1 -- b=2; }
@test "empty-base" { check "a 1\n" -- a=1; }
@test "empty-override" { check "a 1\n" a=1; }
@test "both-empty" { check ""; }
@test "same-keys" { check "a 9\nb 8\n" a=1 b=2 -- a=9 b=8; }
@test "zero-wins" { check "a 0\n" a=5 -- a=0; }
@test "case" { check "A 2\na 1\n" a=1 -- A=2; }
