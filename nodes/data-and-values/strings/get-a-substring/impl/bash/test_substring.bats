#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-substring/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/substring.sh"
}

check() { # check WANT S START END
  run substring "$2" "$3" "$4"
  [ "$status" -eq 0 ]
  [ "$output" = "$1" ]
}

@test "middle" { check 'world' 'Hello, world' 7 12; }
@test "prefix" { check 'Hello' 'Hello, world' 0 5; }
@test "one-char" { check 'b' 'abc' 1 2; }
@test "empty" { check '' '' 0 0; }
@test "whole" { check 'abc' 'abc' 0 3; }
@test "empty-range" { check '' 'abc' 1 1; }
@test "at-end" { check '' 'abc' 3 3; }
# C3 AF is U+00EF, and C3 A9 is U+00E9.
@test "non-ascii" { check $'\xc3\xafve' $'na\xc3\xafve caf\xc3\xa9' 2 5; }
@test "cjk" { check 'テキスト' '日本語テキスト' 3 7; }
@test "astral" { check '🙂b🙃' 'a🙂b🙃c' 1 4; }
@test "astral-only" { check '🙃' '🙂🙃' 1 2; }
# CC 81 is U+0301 COMBINING ACUTE ACCENT.
@test "combining" { check 'e' $'e\xcc\x81t\xc3\xa9' 0 1; }
@test "combining-mark" { check $'\xcc\x81t' $'e\xcc\x81t\xc3\xa9' 1 3; }
