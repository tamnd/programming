# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/empty.sh"
}

check() { # check STATUS FUNCTION S
  run "$2" "$3"
  [ "$status" -eq "$1" ]
  [ "$output" = "" ]
}

# C2 A0 is U+00A0 NO-BREAK SPACE, E2 80 8B is U+200B, and E3 80 80 is U+3000.
@test "letter" { check 1 is_empty 'a'; }
@test "word" { check 1 is_empty 'hello'; }
@test "empty" { check 0 is_empty ''; }
@test "space" { check 1 is_empty ' '; }
@test "newline" { check 1 is_empty $'\n'; }
@test "nbsp" { check 1 is_empty $'\xc2\xa0'; }
@test "zero-width" { check 1 is_empty $'\xe2\x80\x8b'; }
@test "combining" { check 1 is_empty $'\xcc\x81'; }
@test "astral" { check 1 is_empty $'\xf0\x9f\x99\x82'; }
@test "cjk" { check 1 is_empty '日'; }
@test "blank-empty" { check 0 is_blank ''; }
@test "blank-space" { check 0 is_blank ' '; }
@test "blank-mixed" { check 0 is_blank $' \t\n\r'; }
@test "blank-vt-ff" { check 0 is_blank $'\v\f'; }
@test "blank-letter" { check 1 is_blank 'a'; }
@test "blank-padded" { check 1 is_blank '  a  '; }
@test "blank-nbsp" { check 1 is_blank $'\xc2\xa0'; }
@test "blank-zero-width" { check 1 is_blank $'\xe2\x80\x8b'; }
@test "blank-ideographic" { check 1 is_blank $'\xe3\x80\x80'; }
@test "blank-astral" { check 1 is_blank $' \xf0\x9f\x99\x82 '; }
