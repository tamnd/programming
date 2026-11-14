#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-set-operations/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/compare_sets.sh"
}

check() { # check WANT ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(compare_sets "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "colors" { check "blue green red yellow\nblue\ngreen red\ngreen red yellow\nfalse\n" red blue green -- blue yellow; }
@test "subset" { check "a b c\nb\n\na c\ntrue\n" b -- a b c; }
@test "both-empty" { check "\n\n\n\ntrue\n" --; }
@test "empty-a" { check "x\n\n\nx\ntrue\n" -- x; }
@test "empty-b" { check "x\n\nx\nx\nfalse\n" x --; }
@test "equal" { check "a b\na b\n\n\ntrue\n" a b -- b a; }
@test "disjoint" { check "a b\n\na\na b\nfalse\n" a -- b; }
@test "duplicates" { check "a b\nb\na\na\nfalse\n" a a b -- b b; }
@test "case" { check "A a\n\na\nA a\nfalse\n" a -- A; }
