#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-length-of-a-string/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/string_length.sh"
}

check() { # check STRING WANT
  run lengths "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

@test "ascii" { check 'hello' '5 5 5'; }
@test "words" { check 'Hello, world' '12 12 12'; }
@test "two-byte" { check 'naïve' '6 5 5'; }
@test "three-byte" { check '日本語' '9 3 3'; }
@test "cyrillic" { check 'Привет' '12 6 6'; }
@test "empty" { check '' '0 0 0'; }
@test "single" { check 'a' '1 1 1'; }
@test "euro-sign" { check '€' '3 1 1'; }
@test "astral" { check '🙂' '4 2 1'; }
@test "mixed" { check 'a🙂b' '6 4 3'; }
# U+0301 COMBINING ACUTE ACCENT is CC 81 in UTF-8.
@test "combining-mark" { check $'e\xcc\x81' '3 2 2'; }
@test "precomposed" { check $'\xc3\xa9' '2 1 1'; }
@test "flag" { check '🇫🇷' '8 4 2'; }
# U+200D ZERO WIDTH JOINER is E2 80 8D in UTF-8.
@test "zwj-family" { check $'👨\xe2\x80\x8d👩\xe2\x80\x8d👧' '18 8 5'; }
# A Bash variable cannot hold a NUL character, so there is no nul case.
