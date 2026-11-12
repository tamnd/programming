# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-strings/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/compare.sh"
}

order() { # order WANT A B
  run compare "$2" "$3"
  [ "$status" -eq 0 ]
  [ "$output" = "$1" ]
}

equal() { # equal STATUS A B
  run equals "$2" "$3"
  [ "$status" -eq "$1" ]
  [ "$output" = "" ]
}

# EF BD A1 is U+FF61, and F0 9F 99 82 is U+1F642. The UTF-8 bytes have the order of the code points.
@test "less" { order -1 'apple' 'banana'; }
@test "equal" { order 0 'apple' 'apple'; }
@test "greater" { order 1 'pear' 'apple'; }
@test "both-empty" { order 0 '' ''; }
@test "empty-first" { order -1 '' 'a'; }
@test "prefix" { order -1 'app' 'apple'; }
@test "prefix-longer" { order 1 'apple' 'app'; }
@test "upper-before-lower" { order -1 'Zebra' 'apple'; }
@test "lower-after-upper" { order 1 'a' 'A'; }
@test "digits" { order -1 '10' '9'; }
@test "non-ascii" { order 1 $'\xc3\xa9' 'z'; }
@test "astral-after-bmp" { order -1 $'\xef\xbd\xa1' $'\xf0\x9f\x99\x82'; }
@test "astral" { order -1 $'\xf0\x9f\x99\x82' $'\xf0\x9f\x99\x83'; }
@test "combining" { order -1 $'cafe\xcc\x81' $'caf\xc3\xa9'; }
@test "eq-same" { equal 0 'abc' 'abc'; }
@test "eq-case" { equal 1 'abc' 'ABC'; }
@test "eq-empty" { equal 0 '' ''; }
@test "eq-combining" { equal 1 $'cafe\xcc\x81' $'caf\xc3\xa9'; }
@test "eq-astral" { equal 0 $'\xf0\x9f\x99\x82' $'\xf0\x9f\x99\x82'; }
@test "eq-trailing-space" { equal 1 'abc' 'abc '; }
