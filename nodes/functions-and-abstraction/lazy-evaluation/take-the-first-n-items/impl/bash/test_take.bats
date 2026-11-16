#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/take-the-first-n-items/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/take.sh"
}

check() { # check WANT COMMAND ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$("$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "first-three" { check "1\n2\n3\n" take 3 1 2 3 4 5; }
@test "first-two" { check "10\n20\n" take 2 10 20 30 40; }
@test "zero" { check "" take 0 1 2; }
@test "empty" { check "" take 3; }
@test "more-than-length" { check "1\n2\n" take 5 1 2; }
@test "equal-length" { check "4\n5\n6\n" take 3 4 5 6; }
@test "one" { check "9\n" take 1 9 8; }
@test "negative" { check "-1\n-2\n" take 2 -1 -2 -3; }
@test "extremes" { check "-2147483648\n2147483647\n" take 2 -2147483648 2147483647 0; }
@test "naturals from-zero" { check "0\n1\n2\n3\n4\n" take_naturals 0 5; }
@test "naturals from-ten" { check "10\n11\n12\n" take_naturals 10 3; }
@test "naturals none" { check "" take_naturals 7 0; }
@test "naturals one" { check "0\n" take_naturals 0 1; }
@test "naturals large-start" { check "999998\n999999\n" take_naturals 999998 2; }
