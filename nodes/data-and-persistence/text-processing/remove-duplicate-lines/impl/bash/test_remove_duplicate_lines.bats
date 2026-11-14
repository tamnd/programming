#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicate-lines/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/remove_duplicate_lines.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES WANT
  printf '%b' "$1" >"$file"
  # The x keeps the final LFs, which $(...) removes.
  got=$(remove_duplicate_lines "$file" && printf x)
  [ "$got" = "$(printf '%bx' "$2")" ]
}

@test "simple" { check 'b\na\nb\nc\na\n' 'b\na\nc\n'; }
@test "adjacent" { check 'a\na\nb\n' 'a\nb\n'; }
@test "no-duplicates" { check 'c\nb\na\n' 'c\nb\na\n'; }
@test "empty" { check '' ''; }
@test "empty-lines" { check '\na\n\n\n' '\na\n'; }
@test "all-same" { check 'x\nx\nx\n' 'x\n'; }
@test "no-final-newline" { check 'a\nb\na' 'a\nb\n'; }
@test "crlf" { check 'a\r\na\n' 'a\r\na\n'; }
@test "case" { check 'A\na\nA\n' 'A\na\n'; }
@test "whitespace" { check 'a \na\n a\na \n' 'a \na\n a\n'; }
@test "normalization" { check '\xc3\xa9\ne\xcc\x81\n\xc3\xa9\n' '\xc3\xa9\ne\xcc\x81\n'; }

@test "many-lines" {
  seq 0 99999 | awk '{ print $1 % 1000 }' >"$file"
  [ "$(remove_duplicate_lines "$file")" = "$(seq 0 999)" ]
}

@test "not-found" {
  run remove_duplicate_lines "$BATS_TEST_TMPDIR/missing.txt"
  [ "$status" -ne 0 ]
  [ "$output" = "remove_duplicate_lines: cannot read $BATS_TEST_TMPDIR/missing.txt" ]
}
