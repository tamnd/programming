#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-an-enumeration/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/cycle.sh"
}

check() { # check WANT START STEPS
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(cycle "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "weekdays" { check "monday\ntuesday\nwednesday\n" monday 3; }
@test "wrap" { check "friday\nsaturday\nsunday\nmonday\n" friday 4; }
@test "zero-steps" { check "" monday 0; }
@test "more-than-a-week" { check "sunday\nmonday\ntuesday\nwednesday\nthursday\nfriday\nsaturday\nsunday\nmonday\n" sunday 9; }
@test "unknown" { check "" funday 3; }
@test "wrong-case" { check "" Friday 2; }
@test "number" { check "" 3 2; }
@test "empty-name" { check "" "" 1; }
