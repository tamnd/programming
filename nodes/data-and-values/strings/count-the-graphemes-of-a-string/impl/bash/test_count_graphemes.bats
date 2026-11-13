# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/count_graphemes.sh"
}

check() { # check C WANT
  run count_graphemes "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

# Each argument is a string of UTF-8 bytes.
@test "empty" { check '' 0; }
@test "ascii" { check 'abc' 3; }
@test "precomposed" { check $'\xc3\xa9' 1; }
@test "emoji" { check $'\xf0\x9f\x99\x82' 1; }
@test "combining" { check $'e\xcc\x81' 1; }
@test "two-marks" { check $'e\xcc\x81\xcc\x82' 1; }
@test "leading-mark" { check $'\xcc\x81a' 2; }
@test "crlf" { check $'\r\n' 1; }
@test "lfcr" { check $'\n\r' 2; }
@test "cr-mark" { check $'\r\xcc\x81' 2; }
@test "soft-hyphen" { check $'a\xc2\xadb' 3; }
@test "flag" { check $'\xf0\x9f\x87\xab\xf0\x9f\x87\xb7' 1; }
@test "three-indicators" { check $'\xf0\x9f\x87\xab\xf0\x9f\x87\xb7\xf0\x9f\x87\xaa' 2; }
@test "two-flags" { check $'\xf0\x9f\x87\xab\xf0\x9f\x87\xb7\xf0\x9f\x87\xaa\xf0\x9f\x87\xb8' 2; }
@test "zwj-sequence" { check $'\xf0\x9f\x91\xa9\xe2\x80\x8d\xf0\x9f\x92\xbb' 1; }
@test "zwj-after-letter" { check $'a\xe2\x80\x8d\xf0\x9f\x99\x82' 2; }
@test "skin-tone" { check $'\xf0\x9f\x91\x8d\xf0\x9f\x8f\xbd' 1; }
@test "variation-selector" { check $'\xe2\x9d\xa4\xef\xb8\x8f' 1; }
