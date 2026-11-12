# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/to_bytes.sh"
}

check() { # check S WANT
  run to_bytes "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

# Each argument is a string of UTF-8 bytes, and the output is the byte values with one space between them.
@test "ascii" { check 'abc' '97 98 99'; }
@test "letter" { check 'A' '65'; }
@test "punct" { check 'hi!' '104 105 33'; }
@test "empty" { check '' ''; }
@test "control" { check $'\t\n' '9 10'; }
@test "two-byte" { check $'\xc3\xa9' '195 169'; }
@test "latin-1-max" { check $'\xc3\xbf' '195 191'; }
@test "three-byte" { check $'\xe2\x82\xac' '226 130 172'; }
@test "bom" { check $'\xef\xbb\xbf' '239 187 191'; }
@test "astral" { check $'\xf0\x9f\x99\x82' '240 159 153 130'; }
@test "combining" { check $'e\xcc\x81' '101 204 129'; }
@test "mixed" { check $'a\xc3\xa9\xf0\x9f\x99\x82' '97 195 169 240 159 153 130'; }
