#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-with-a-custom-key/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/sort_by_length.sh"
}

check() { # check WANT WORD...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(sort_by_length "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "fruits" { check "fig\nkiwi\npear\napple\n" pear fig apple kiwi; }
@test "three" { check "a\nbb\nccc\n" bb a ccc; }
@test "empty" { check ""; }
@test "single" { check "x\n" x; }
@test "same-length" { check "ant\ncat\ndog\n" dog cat ant; }
@test "duplicates" { check "a\nb\nb\n" b a b; }
@test "sorted" { check "a\nbb\nccc\n" a bb ccc; }
@test "reverse" { check "a\nbb\nccc\n" ccc bb a; }
@test "long" { check "z\nabcdefghi\nabcdefghij\n" abcdefghij abcdefghi z; }
@test "prefix" { check "a\nb\nab\nabc\n" ab a abc b; }
