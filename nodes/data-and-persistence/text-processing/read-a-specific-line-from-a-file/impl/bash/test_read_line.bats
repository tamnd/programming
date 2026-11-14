#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/read_line.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES NUMBER WANT
  printf '%b' "$1" >"$file"
  # The x keeps the final LF, which $(...) removes.
  got=$(read_line "$file" "$2" && printf x)
  [ "$got" = "$(printf '%bx' "$3")" ]
}

check_none() { # check_none BYTES NUMBER
  printf '%b' "$1" >"$file"
  run read_line "$file" "$2"
  [ "$status" -eq 1 ] && [ "$output" = "" ]
}

@test "first" { check 'a\nb\nc\n' 1 'a\n'; }
@test "middle" { check 'a\nb\nc\n' 2 'b\n'; }
@test "last" { check 'a\nb\nc\n' 3 'c\n'; }
@test "past-the-end" { check_none 'a\nb\n' 3; }
@test "zero" { check_none 'a\n' 0; }
@test "negative" { check_none 'a\n' -1; }
@test "empty" { check_none '' 1; }
@test "empty-line" { check 'a\n\nb\n' 2 '\n'; }
@test "only-newline" { check '\n' 1 '\n'; }
@test "no-final-newline" { check 'a\nb' 2 'b\n'; }
@test "crlf" { check 'a\r\nb\r\n' 1 'a\r\n'; }
@test "lone-cr" { check 'a\rb\nc\n' 2 'c\n'; }
@test "non-ascii" { check 'caf\xc3\xa9\nzo\xc3\xab\n' 2 'zo\xc3\xab\n'; }

@test "many-lines" {
  seq 0 99999 >"$file"
  [ "$(read_line "$file" 100000)" = "99999" ]
  run read_line "$file" 100001
  [ "$status" -eq 1 ]
}

@test "not-found" {
  run read_line "$BATS_TEST_TMPDIR/missing.txt" 1
  [ "$status" -eq 2 ]
  [ "$output" = "read_line: cannot read $BATS_TEST_TMPDIR/missing.txt" ]
}
