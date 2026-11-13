#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-cartesian-product/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/cartesian_product.sh"
}

check() { # check WANT LIST...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(cartesian_product "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "two" { check "1 3\n1 4\n2 3\n2 4\n" "1 2" "3 4"; }
@test "three" { check "1 3 4\n1 3 5\n2 3 4\n2 3 5\n" "1 2" "3" "4 5"; }
@test "no-lists" { check "\n"; }
@test "one-empty" { check "" "1 2" ""; }
@test "single" { check "7\n8\n9\n" "7 8 9"; }
@test "duplicates" { check "1 2\n1 2\n" "1 1" "2"; }
@test "negative" { check "-1 -2\n-1 -3\n" "-1" "-2 -3"; }
@test "extremes" { check "-2147483648 0\n2147483647 0\n" "-2147483648 2147483647" "0"; }
