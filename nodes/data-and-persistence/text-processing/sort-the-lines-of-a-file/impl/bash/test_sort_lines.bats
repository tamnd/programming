#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/sort_lines.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES WANT
  printf '%b' "$1" >"$file"
  # The x keeps the final LFs, which $(...) removes.
  got=$(sort_lines "$file" && printf x)
  [ "$got" = "$(printf '%bx' "$2")" ]
}

@test "simple" { check 'pear\napple\nfig\n' 'apple\nfig\npear\n'; }
@test "case" { check 'b\nB\na\nA\n' 'A\nB\na\nb\n'; }
@test "numbers" { check '10\n9\n100\n' '10\n100\n9\n'; }
@test "empty" { check '' ''; }
@test "one-empty-line" { check '\n' '\n'; }
@test "empty-lines" { check 'b\n\na\n\n' '\n\na\nb\n'; }
@test "duplicates" { check 'b\na\nb\n' 'a\nb\nb\n'; }
@test "no-final-newline" { check 'b\na' 'a\nb\n'; }
@test "crlf" { check 'b\r\na\r\n' 'a\r\nb\r\n'; }
@test "prefix" { check 'ab\na\n' 'a\nab\n'; }
@test "whitespace" { check 'b\n a\n\tc\n' '\tc\n a\nb\n'; }
@test "non-ascii" { check '\xc3\xa9\nz\ne\n' 'e\nz\n\xc3\xa9\n'; }
@test "supplementary" { check '\xf0\x9f\x98\x80\n\xef\xbd\xa1\n' '\xef\xbd\xa1\n\xf0\x9f\x98\x80\n'; }
@test "other-line-breaks" { check 'b\xe2\x80\xa8a\nb\x1ca\nb\x0ca\n' 'b\x0ca\nb\x1ca\nb\xe2\x80\xa8a\n'; }

@test "many-lines" {
  yes 'b
a' | head -n 100000 >"$file"
  [ "$(sort_lines "$file" | uniq -c | awk '{ print $2, $1 }')" = "$(printf 'a 50000\nb 50000')" ]
}

@test "not-found" {
  run sort_lines "$BATS_TEST_TMPDIR/missing.txt"
  [ "$status" -ne 0 ]
  [ "$output" = "sort_lines: cannot read $BATS_TEST_TMPDIR/missing.txt" ]
}
