#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-file-line-by-line/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/line_stats.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES WANT
  printf '%b' "$1" >"$file"
  run line_stats "$file"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

@test "lf" { check 'alpha\nbeta\ngamma\n' "3 5"; }
@test "blank-lines" { check 'first\n\n\nlast line\n' "4 9"; }
@test "empty" { check '' "0 0"; }
@test "only-newline" { check '\n' "1 0"; }
@test "no-final-newline" { check 'one\ntwo\nthree' "3 5"; }
@test "crlf" { check 'one\r\ntwo\r\nthree\r\n' "3 5"; }
@test "crlf-no-final-newline" { check 'one\r\ntwo\r\nthree' "3 5"; }
@test "mixed-endings" { check 'one\r\ntwo\nthree\r\n' "3 5"; }
@test "whitespace" { check '  two spaces\ttab \n' "1 17"; }
@test "backslashes" { check 'C:\\temp\\new\n' "1 11"; }
@test "non-ascii" { check 'naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n' "3 10"; }

@test "long-line" {
  { head -c 1048576 /dev/zero | tr '\0' x; printf '\nend\n'; } >"$file"
  run line_stats "$file"
  [ "$status" -eq 0 ]
  [ "$output" = "2 1048576" ]
}

@test "missing" {
  run --separate-stderr line_stats "$BATS_TEST_TMPDIR/missing.txt"
  [ "$status" -ne 0 ]
  [ -z "$output" ]
}

invalid() { # invalid BYTES
  printf '%b' "$1" >"$file"
  run --separate-stderr line_stats "$file"
  [ "$status" -ne 0 ]
  [ -z "$output" ]
}

@test "invalid-byte" { invalid 'ok\n\xff\xfe\n'; }
@test "truncated-at-end" { invalid 'ok\ncaf\xc3'; }
@test "encoded-surrogate" { invalid 'a\xed\xa0\x80b\n'; }
@test "overlong" { invalid '\xc0\xaf\n'; }
