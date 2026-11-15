#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-with-a-step/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/count_by.sh"
}

check() { # check START STOP STEP -- WANT...
  local start=$1 stop=$2 step=$3
  shift 4
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(count_by "$start" "$stop" "$step"; printf x)
  want=$(if (($# > 0)); then printf '%s\n' "$@"; fi; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "by-two" { check 2 8 2 -- 2 4 6 8; }
@test "down" { check 10 0 -1 -- 10 9 8 7 6 5 4 3 2 1 0; }
@test "uneven" { check 1 10 3 -- 1 4 7 10; }
@test "past-stop" { check 0 9 4 -- 0 4 8; }
@test "down-by-three" { check 10 1 -3 -- 10 7 4 1; }
@test "empty-up" { check 5 1 1 --; }
@test "empty-down" { check 1 5 -1 --; }
@test "single" { check 3 3 7 -- 3; }
@test "negative" { check -3 3 2 -- -3 -1 1 3; }
@test "limit-up" { check 2147483640 2147483647 5 -- 2147483640 2147483645; }
@test "limit-down" { check -2147483640 -2147483648 -5 -- -2147483640 -2147483645; }
@test "big-step" { check 0 10 2147483647 -- 0; }
