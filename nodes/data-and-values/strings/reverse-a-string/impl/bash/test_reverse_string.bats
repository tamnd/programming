#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-string/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/reverse_string.sh"
}

check() { # check STRING WANT
  run reverse_code_points "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

@test "ascii" { check 'hello' 'olleh'; }
@test "two-words" { check 'abc def' 'fed cba'; }
@test "palindrome" { check 'racecar' 'racecar'; }
@test "empty" { check '' ''; }
@test "single" { check 'a' 'a'; }
@test "two-byte" { check 'naïve' 'evïan'; }
@test "three-byte" { check '日本語' '語本日'; }
@test "astral" { check 'a🙂b' 'b🙂a'; }
@test "two-astral" { check '🙂😀' '😀🙂'; }
# U+0301 COMBINING ACUTE ACCENT is CC 81 in UTF-8.
@test "combining-mark" { check $'e\xcc\x81x' $'x\xcc\x81e'; }
@test "flag" { check '🇫🇷' '🇷🇫'; }

invalid() { # invalid BYTES
  run --separate-stderr reverse_code_points "$(printf '%b' "$1")"
  [ "$status" -ne 0 ]
  [ -z "$output" ]
}

@test "invalid-byte" { invalid '\xff\xfe'; }
@test "truncated" { invalid 'caf\xc3'; }
@test "encoded-surrogate" { invalid 'a\xed\xa0\x80b'; }
@test "overlong" { invalid '\xc0\xaf'; }
