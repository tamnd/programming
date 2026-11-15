#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-with-an-index/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/positions.sh"
}

check() { # check TARGET N ITEM_1 ... ITEM_N WANT...
  local target=$1 n=$2
  shift 2
  local -a items=("${@:1:n}")
  shift "$n"
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(positions "$target" "${items[@]}"; printf x)
  want=$(if (($# > 0)); then printf '%s\n' "$@"; fi; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "two-matches" { check a 4 a b a c 0 2; }
@test "words" { check green 4 red green blue green 1 3; }
@test "empty" { check a 0; }
@test "none" { check z 2 a b; }
@test "all" { check x 3 x x x 0 1 2; }
@test "last" { check c 3 a b c 2; }
@test "case" { check a 2 A a 1; }
@test "empty-string" { check "" 3 "" a "" 0 2; }
