# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/code_point.sh"
}

check() { # check C WANT
  run code_point "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

# Each argument is a string of UTF-8 bytes for one code point.
@test "upper" { check 'A' 65; }
@test "lower" { check 'a' 97; }
@test "digit" { check '0' 48; }
@test "space" { check ' ' 32; }
@test "tab" { check $'\t' 9; }
@test "tilde" { check '~' 126; }
@test "two-byte" { check $'\xc3\xa9' 233; }
@test "three-byte" { check $'\xe2\x82\xac' 8364; }
@test "cjk" { check $'\xe6\x97\xa5' 26085; }
@test "combining" { check $'\xcc\x81' 769; }
@test "astral" { check $'\xf0\x9f\x99\x82' 128578; }
@test "max" { check $'\xf4\x8f\xbf\xbf' 1114111; }
