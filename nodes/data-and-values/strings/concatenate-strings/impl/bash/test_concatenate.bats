#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-strings/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/concatenate.sh"
}

check() { # check WANT COMMAND ARGS...
  local want=$1
  shift
  run "$@"
  [ "$status" -eq 0 ]
  [ "$output" = "$want" ]
}

@test "hello-world" { check 'Hello, world' concat 'Hello, ' 'world'; }
@test "two-words" { check 'goodbye' concat 'good' 'bye'; }
@test "keep-spaces" { check 'a  b' concat 'a ' ' b'; }
@test "empty-both" { check '' concat '' ''; }
@test "empty-left" { check 'abc' concat '' 'abc'; }
@test "empty-right" { check 'abc' concat 'abc' ''; }
@test "non-ascii" { check 'naïve' concat 'naï' 've'; }
@test "astral" { check 'a🙂' concat 'a' '🙂'; }
# U+0301 COMBINING ACUTE ACCENT is CC 81 in UTF-8.
@test "combining-mark" { check $'e\xcc\x81' concat 'e' $'\xcc\x81'; }
@test "flag-halves" { check '🇫🇷' concat '🇫' '🇷'; }
@test "digit-strings" { check '12' concat '1' '2'; }
@test "number positive" { check 'item3' concat_number 'item' 3; }
@test "number zero" { check 'n=0' concat_number 'n=' 0; }
@test "number negative" { check 'x-5' concat_number 'x' -5; }
@test "number empty-prefix" { check '42' concat_number '' 42; }
@test "number int32-max" { check 'max=2147483647' concat_number 'max=' 2147483647; }
@test "number int32-min" { check 'min=-2147483648' concat_number 'min=' -2147483648; }
