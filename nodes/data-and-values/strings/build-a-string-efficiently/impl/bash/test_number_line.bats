#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/build-a-string-efficiently/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/number_line.sh"
}

check() { # check WANT N
  run number_line "$2"
  [ "$status" -eq 0 ]
  [ "$output" = "$1" ]
}

check_long() { # check_long N SIZE END: the length, the start and the end
  run number_line "$1"
  [ "$status" -eq 0 ]
  [ "${#output}" -eq "$2" ]
  [ "${output:0:6}" = "1,2,3," ]
  [ "${output: -${#3}}" = "$3" ]
}

@test "zero" { check '' 0; }
@test "one" { check '1' 1; }
@test "three" { check '1,2,3' 3; }
@test "five" { check '1,2,3,4,5' 5; }
@test "seven" { check '1,2,3,4,5,6,7' 7; }
@test "nine" { check '1,2,3,4,5,6,7,8,9' 9; }
@test "ten" { check '1,2,3,4,5,6,7,8,9,10' 10; }
@test "hundred" { check_long 100 291 ',99,100'; }
@test "hundred-and-one" { check_long 101 295 ',100,101'; }
@test "thousand" { check_long 1000 3892 ',999,1000'; }
@test "large" { check_long 100000 588894 ',99999,100000'; }
