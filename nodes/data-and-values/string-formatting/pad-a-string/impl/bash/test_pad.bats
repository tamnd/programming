#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pad-a-string/.

setup() {
  source "$BATS_TEST_DIRNAME/pad.sh"
}

@test "zeros" { run pad_left '7' 3 '0'; [ "$output" = '007' ]; run pad_right '7' 3 '0'; [ "$output" = '700' ]; }
@test "spaces" { run pad_left 'abc' 6 ' '; [ "$output" = '   abc' ]; run pad_right 'abc' 6 ' '; [ "$output" = 'abc   ' ]; }
@test "stars" { run pad_left 'abc' 6 '*'; [ "$output" = '***abc' ]; run pad_right 'abc' 6 '*'; [ "$output" = 'abc***' ]; }
@test "empty-string" { run pad_left '' 3 '-'; [ "$output" = '---' ]; run pad_right '' 3 '-'; [ "$output" = '---' ]; }
@test "width-zero" { run pad_left 'abc' 0 ' '; [ "$output" = 'abc' ]; run pad_right 'abc' 0 ' '; [ "$output" = 'abc' ]; }
@test "exact-width" { run pad_left 'abcd' 4 '.'; [ "$output" = 'abcd' ]; run pad_right 'abcd' 4 '.'; [ "$output" = 'abcd' ]; }
@test "shorter-width" { run pad_left 'hello' 3 ' '; [ "$output" = 'hello' ]; run pad_right 'hello' 3 ' '; [ "$output" = 'hello' ]; }
@test "negative-number-text" { run pad_left '-42' 6 '0'; [ "$output" = '000-42' ]; run pad_right '-42' 6 '0'; [ "$output" = '-42000' ]; }
@test "fill-same-as-text" { run pad_left 'aa' 5 'a'; [ "$output" = 'aaaaa' ]; run pad_right 'aa' 5 'a'; [ "$output" = 'aaaaa' ]; }
@test "one-char-wide" { run pad_left 'x' 1 '_'; [ "$output" = 'x' ]; run pad_right 'x' 1 '_'; [ "$output" = 'x' ]; }
@test "spaces-inside" { run pad_left 'a b' 7 '.'; [ "$output" = '....a b' ]; run pad_right 'a b' 7 '.'; [ "$output" = 'a b....' ]; }
