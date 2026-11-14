#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-word-count/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/word_count.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES WANT
  printf '%b' "$1" >"$file"
  run word_count "$file"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

invalid() { # invalid BYTES
  printf '%b' "$1" >"$file"
  run --separate-stderr word_count "$file"
  [ "$status" -ne 0 ]
  [ -z "$output" ]
}

@test "simple" { check 'one two three\n' "1 3 14"; }
@test "multiple-spaces" { check '  a   b  \n' "1 2 10"; }
@test "tabs-and-lines" { check 'a\tb\nc\n' "2 3 6"; }
@test "empty" { check '' "0 0 0"; }
@test "only-white-space" { check ' \n\t\n' "2 0 4"; }
@test "no-final-newline" { check 'one two' "0 2 7"; }
@test "crlf" { check 'a b\r\nc\r\n' "2 3 8"; }
@test "non-ascii" { check 'na\xc3\xafve caf\xc3\xa9\n' "1 2 13"; }
@test "unicode-spaces" { check 'a\xc2\xa0b\xe3\x80\x80c\xe2\x80\xa8d\n' "1 4 13"; }
@test "not-white-space" { check 'a\xe2\x80\x8bb\xc2\xadc\xef\xbb\xbfd\n' "1 1 13"; }
@test "control-separators" { check 'a\x1cb\x1fc\n' "1 1 6"; }
@test "nel" { check 'a\xc2\x85b\n' "1 2 5"; }

@test "many-lines" {
  yes 'a b' | head -n 100000 >"$file"
  run word_count "$file"
  [ "$status" -eq 0 ]
  [ "$output" = "100000 200000 400000" ]
}

@test "invalid-bytes" { invalid 'ok\xff\xfe\n'; }
@test "truncated" { invalid 'ok \xe2\x82'; }
@test "surrogate" { invalid 'a\xed\xa0\x80b\n'; }

@test "missing" {
  run --separate-stderr word_count "$BATS_TEST_TMPDIR/missing.txt"
  [ "$status" -ne 0 ]
  [ -z "$output" ]
}
