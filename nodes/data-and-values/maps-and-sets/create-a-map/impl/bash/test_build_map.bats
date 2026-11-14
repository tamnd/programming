#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-map/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/build_map.sh"
}

check() { # check WANT ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(build_map "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "fruits" { check "apple 3\nfig 2\npear 5\n" apple=3 pear=5 fig=2; }
@test "update-and-delete" { check "a 7\nc 3\n" a=1 b=2 a=7 c=3 -- b; }
@test "empty" { check ""; }
@test "replace" { check "k 3\n" k=1 k=2 k=3; }
@test "delete-absent" { check "a 1\n" a=1 -- z; }
@test "delete-all" { check "" a=1 b=2 -- a b; }
@test "delete-twice" { check "b 2\n" a=1 b=2 -- a a; }
@test "zero-value" { check "a 0\nb -4\n" a=0 b=-4; }
@test "case" { check "A 4\nB 2\na 3\nb 1\n" b=1 B=2 a=3 A=4; }
