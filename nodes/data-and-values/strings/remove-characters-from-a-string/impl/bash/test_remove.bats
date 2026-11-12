# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-characters-from-a-string/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/remove.sh"
}

check() { # check S CHARS WANT
  run remove_chars "$1" "$2"
  [ "$status" -eq 0 ]
  [ "$output" = "$3" ]
}

# Each argument is a string of UTF-8 bytes, and the function removes whole code points.
@test "one-char" { check 'banana' 'a' 'bnn'; }
@test "set" { check 'hello, world' 'lo' 'he, wrd'; }
@test "punct" { check 'a-b_c.d' '-_.' 'abcd'; }
@test "empty" { check '' 'abc' ''; }
@test "empty-set" { check 'hello' '' 'hello'; }
@test "none-found" { check 'hello' 'xyz' 'hello'; }
@test "all" { check 'aaa' 'a' ''; }
@test "adjacent" { check 'aab' 'a' 'b'; }
@test "astral" { check $'a\xf0\x9f\x99\x82b\xf0\x9f\x99\x82' $'\xf0\x9f\x99\x82' 'ab'; }
@test "astral-other" { check $'\xf0\x9f\x99\x82\xf0\x9f\x99\x83' $'\xf0\x9f\x99\x83' $'\xf0\x9f\x99\x82'; }
@test "non-ascii" { check $'\xc3\xa0\xc3\xa9' $'\xc3\xa9' $'\xc3\xa0'; }
@test "combining-mark" { check $'e\xcc\x81x' $'\xcc\x81' 'ex'; }
@test "combining-base" { check $'e\xcc\x81' 'e' $'\xcc\x81'; }
@test "composed" { check $'e\xcc\x81' $'\xc3\xa9' $'e\xcc\x81'; }
