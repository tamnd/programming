#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-whole-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/read_text.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

# "run" drops final newlines from $output, so these tests call read_text directly.
check() { # check BYTES
  local want text
  printf '%b' "$1" >"$file"
  want=$(printf '%b' "$1" && printf x)
  read_text text "$file"
  [ "$text" = "${want%x}" ]
}

@test "lf" { check 'alpha\nbeta\n'; }
@test "paragraphs" { check 'first line\n\nsecond paragraph\n'; }
@test "non-ascii" { check 'naïve café\n日本語\n🙂\n'; }
@test "empty" { check ''; }
@test "no-final-newline" { check 'one\ntwo'; }
@test "trailing-newlines" { check 'end\n\n\n'; }
@test "crlf" { check 'one\r\ntwo\r\n'; }
@test "mixed-endings" { check 'lf\ncrlf\r\ncr\rend'; }
@test "bom" { check '\xef\xbb\xbfhead\n'; }
@test "whitespace" { check '  two spaces\ttab \n'; }

@test "large" {
  local text want
  want=$(printf '0123456789abcdef%.0s' {1..65536})
  printf '%s' "$want" >"$file"
  read_text text "$file"
  [ "$text" = "$want" ]
}

@test "missing" {
  run ! read_text text "$BATS_TEST_TMPDIR/missing.txt"
}

@test "directory" {
  run ! read_text text "$BATS_TEST_TMPDIR"
}

@test "invalid-byte" { printf 'ok\n\xff\xfe\n' >"$file"; run ! read_text text "$file"; }
@test "truncated-at-end" { printf 'ok\ncaf\xc3' >"$file"; run ! read_text text "$file"; }
@test "encoded-surrogate" { printf 'a\xed\xa0\x80b\n' >"$file"; run ! read_text text "$file"; }
@test "overlong" { printf '\xc0\xaf\n' >"$file"; run ! read_text text "$file"; }
