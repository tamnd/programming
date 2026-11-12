# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/change-the-case-of-a-string/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/case.sh"
}

check() { # check FUNCTION S WANT
  run "$1" "$2"
  [ "$status" -eq 0 ]
  [ "$output" = "$3" ]
}

# Only the ASCII letters change, so each byte of 0x80 or more stays as it is.
@test "word" { check to_upper 'hello' 'HELLO'; }
@test "mixed" { check to_upper 'Hello, World!' 'HELLO, WORLD!'; }
@test "already" { check to_upper 'ABC' 'ABC'; }
@test "empty" { check to_upper '' ''; }
@test "digits-punct" { check to_upper 'a1-b2_c3' 'A1-B2_C3'; }
@test "non-ascii" { check to_upper $'caf\xc3\xa9' $'CAF\xc3\xa9'; }
@test "sharp-s" { check to_upper $'stra\xc3\x9fe' $'STRA\xc3\x9fE'; }
@test "dotless-i" { check to_upper $'\xc4\xb1 i' $'\xc4\xb1 I'; }
@test "greek" { check to_upper $'\xcf\x83' $'\xcf\x83'; }
@test "fullwidth" { check to_upper $'\xef\xbd\x81' $'\xef\xbd\x81'; }
@test "astral" { check to_upper $'a\xf0\x9f\x99\x82b' $'A\xf0\x9f\x99\x82B'; }
@test "combining" { check to_upper $'cafe\xcc\x81' $'CAFE\xcc\x81'; }
@test "lower-word" { check to_lower 'HELLO' 'hello'; }
@test "lower-mixed" { check to_lower 'Hello, World!' 'hello, world!'; }
@test "lower-empty" { check to_lower '' ''; }
@test "lower-dotted-i" { check to_lower $'\xc4\xb0' $'\xc4\xb0'; }
@test "lower-sigma" { check to_lower $'\xce\xa3\xce\x91\xce\xa3' $'\xce\xa3\xce\x91\xce\xa3'; }
@test "lower-kelvin" { check to_lower $'\xe2\x84\xaa' $'\xe2\x84\xaa'; }
@test "lower-astral" { check to_lower $'A\xf0\x9f\x99\x82B' $'a\xf0\x9f\x99\x82b'; }
