# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/prefix.sh"
}

check() { # check STATUS FUNCTION S AFFIX
  run "$2" "$3" "$4"
  [ "$status" -eq "$1" ]
  [ "$output" = "" ]
}

# CC 81 is U+0301 COMBINING ACUTE ACCENT, and C3 A9 is U+00E9.
@test "yes" { check 0 starts_with 'Hello, world' 'Hello'; }
@test "no" { check 1 starts_with 'Hello, world' 'world'; }
@test "whole" { check 0 starts_with 'abc' 'abc'; }
@test "empty-prefix" { check 0 starts_with 'abc' ''; }
@test "empty-s" { check 1 starts_with '' 'a'; }
@test "both-empty" { check 0 starts_with '' ''; }
@test "longer-prefix" { check 1 starts_with 'ab' 'abc'; }
@test "case-sensitive" { check 1 starts_with 'Hello' 'hello'; }
@test "non-ascii" { check 0 starts_with $'caf\xc3\xa9 au lait' $'caf\xc3\xa9'; }
@test "cjk" { check 0 starts_with '日本語' '日本'; }
@test "astral" { check 0 starts_with $'\xf0\x9f\x99\x82\xf0\x9f\x99\x83' $'\xf0\x9f\x99\x82'; }
@test "astral-no" { check 1 starts_with $'\xf0\x9f\x99\x82' $'\xf0\x9f\x99\x83'; }
@test "combining" { check 0 starts_with $'cafe\xcc\x81' 'cafe'; }
@test "precomposed" { check 1 starts_with $'caf\xc3\xa9' 'cafe'; }
@test "ends-yes" { check 0 ends_with 'report.txt' '.txt'; }
@test "ends-no" { check 1 ends_with 'report.txt' '.csv'; }
@test "ends-empty" { check 0 ends_with 'abc' ''; }
@test "ends-longer" { check 1 ends_with 'txt' 'a.txt'; }
@test "ends-case" { check 1 ends_with 'A.TXT' '.txt'; }
@test "ends-astral" { check 0 ends_with $'a\xf0\x9f\x99\x82' $'\xf0\x9f\x99\x82'; }
@test "ends-combining" { check 1 ends_with $'cafe\xcc\x81' 'e'; }
