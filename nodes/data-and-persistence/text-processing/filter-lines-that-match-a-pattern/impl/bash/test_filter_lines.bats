#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/filter_lines.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES PATTERN WANT
  printf '%b' "$1" >"$file"
  # The x keeps the final LFs, which $(...) removes.
  got=$(filter_lines "$file" "$2" && printf x)
  [ "$got" = "$(printf '%bx' "$3")" ]
}

@test "literal" { check 'apple\nbanana\ncherry\n' 'an' 'banana\n'; }
@test "several" { check 'cat\ndog\ncatalog\n' 'cat' 'cat\ncatalog\n'; }
@test "start" { check 'ab\nba\n' '^a' 'ab\n'; }
@test "end" { check 'ab\nba\n' 'a$' 'ba\n'; }
@test "dot" { check 'abc\nac\naxc\n' 'a.c' 'abc\naxc\n'; }
@test "star" { check 'ac\nabc\nabbc\nadc\n' 'ab*c' 'ac\nabc\nabbc\n'; }
@test "whole-line" { check 'a\nab\nba\n' '^a$' 'a\n'; }
@test "empty" { check '' 'a' ''; }
@test "no-match" { check 'abc\n' 'x' ''; }
@test "empty-lines" { check 'a\n\nb\n\n' '^$' '\n\n'; }
@test "match-all" { check 'a\n\nb\n' '.*' 'a\n\nb\n'; }
@test "duplicates" { check 'a\nb\na\n' 'a' 'a\na\n'; }
@test "no-final-newline" { check 'a\nab' 'b' 'ab\n'; }
@test "crlf-end" { check 'ab\r\nab\n' 'b$' 'ab\n'; }
@test "crlf-dot" { check 'a\r\n' '^a.$' 'a\r\n'; }
@test "case" { check 'Apple\napple\n' 'apple' 'apple\n'; }
@test "non-ascii" { check 'caf\xc3\xa9\ncafe\n' "$(printf '\xc3\xa9')" 'caf\xc3\xa9\n'; }

@test "many-lines" {
  seq 0 99999 >"$file"
  [ "$(filter_lines "$file" '^9*$')" = "$(printf '9\n99\n999\n9999\n99999')" ]
}

@test "not-found" {
  run filter_lines "$BATS_TEST_TMPDIR/missing.txt" a
  [ "$status" -ne 0 ]
  [ "$output" = "filter_lines: cannot read $BATS_TEST_TMPDIR/missing.txt" ]
}
