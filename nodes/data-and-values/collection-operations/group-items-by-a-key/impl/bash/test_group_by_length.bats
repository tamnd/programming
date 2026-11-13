#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/group-items-by-a-key/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/group_by_length.sh"
}

check() { # check WANT WORD...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(group_by_length "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "fruits" { check "3 fig\n4 kiwi pear plum date\n5 apple\n" apple fig kiwi pear plum date; }
@test "letters" { check "1 a c\n2 bb\n" a bb c; }
@test "empty" { check ""; }
@test "single" { check "1 x\n" x; }
@test "same-length" { check "3 cat dog cow\n" cat dog cow; }
@test "order" { check "1 a\n2 bb\n3 ccc\n" ccc a bb; }
@test "duplicates" { check "1 c\n2 ab ab\n" ab ab c; }
@test "long" { check "1 a\n9 abcdefghi\n10 abcdefghij\n" abcdefghij abcdefghi a; }
