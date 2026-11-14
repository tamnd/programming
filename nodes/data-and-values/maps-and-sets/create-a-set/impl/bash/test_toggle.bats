#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-set/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/toggle.sh"
}

check() { # check WANT ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(toggle "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "colors" { check "blue\ngreen\n" red blue red green; }
@test "fresh" { check "a\nb\n" b a; }
@test "empty" { check ""; }
@test "two-times" { check "" a a; }
@test "three-times" { check "a\n" a a a; }
@test "case" { check "A\na\n" a A; }
@test "prefix" { check "a\nab\n" ab a; }
