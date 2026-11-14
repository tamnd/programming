#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/lookup.sh"
}

check() { # check WANT ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(lookup "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "prices" { check "3\n0\n5\n" 0 apple=3 pear=5 -- apple fig pear; }
@test "negative-fallback" { check "-1\n1\n" -1 a=1 -- b a; }
@test "empty-table" { check "7\n7\n" 7 -- a b; }
@test "empty-keys" { check "" 0 a=1; }
@test "zero-value" { check "0\n5\n" 5 a=0 -- a b; }
@test "repeat" { check "2\n2\n1\n" 2 a=1 -- b b a; }
@test "case" { check "0\n" 0 a=1 -- A; }
