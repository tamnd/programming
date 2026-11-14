#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/count_lines.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES WANT
  printf '%b' "$1" >"$file"
  run count_lines "$file"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

@test "lf" { check 'alpha\nbeta\ngamma\n' "3 3"; }
@test "blank-lines" { check 'first\n\n\nlast\n' "4 4"; }
@test "empty" { check '' "0 0"; }
@test "only-newline" { check '\n' "1 1"; }
@test "no-final-newline" { check 'one\ntwo\nthree' "2 3"; }
@test "one-line-no-newline" { check 'x' "0 1"; }
@test "crlf" { check 'one\r\ntwo\r\n' "2 2"; }
@test "crlf-no-final-newline" { check 'one\r\ntwo' "1 2"; }
@test "cr-only" { check 'one\rtwo\r' "0 1"; }
@test "other-breaks" { check 'a\vb\fc\xc2\x85d\xe2\x80\xa8e\n' "1 1"; }
@test "invalid-utf8" { check 'ok\n\xff\xfe\n' "2 2"; }
@test "nul-bytes" { check 'a\0b\n\0\n' "2 2"; }

@test "many-chunks" {
  { yes x | head -n 100000; printf 'end'; } >"$file"
  run count_lines "$file"
  [ "$status" -eq 0 ]
  [ "$output" = "100000 100001" ]
}

@test "missing" {
  run --separate-stderr count_lines "$BATS_TEST_TMPDIR/missing.txt"
  [ "$status" -ne 0 ]
  [ -z "$output" ]
}
