#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-lines-from-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/remove_lines.sh"
  dir="$BATS_TEST_TMPDIR/case"
  mkdir -p "$dir"
  file="$dir/input.txt"
}

check() { # check BYTES START COUNT WANT AFTER
  printf '%b' "$1" >"$file"
  [ "$(remove_lines "$file" "$2" "$3")" = "$4" ]
  # The x keeps a final LF, which $(...) removes.
  [ "$(cat "$file"; printf x)" = "$(printf '%bx' "$5")" ]
  # No temporary file stays in the directory.
  [ "$(ls -A "$dir")" = "input.txt" ]
}

@test "first-two" { check 'a\nb\nc\nd\n' 1 2 2 'c\nd\n'; }
@test "middle" { check 'a\nb\nc\nd\n' 2 2 2 'a\nd\n'; }
@test "last" { check 'a\nb\nc\n' 3 1 1 'a\nb\n'; }
@test "past-the-end" { check 'a\nb\n' 3 1 0 'a\nb\n'; }
@test "partly-past-the-end" { check 'a\nb\nc\n' 2 5 2 'a\n'; }
@test "count-zero" { check 'a\nb\n' 1 0 0 'a\nb\n'; }
@test "all-lines" { check 'a\nb\n' 1 2 2 ''; }
@test "empty" { check '' 1 1 0 ''; }
@test "empty-line" { check 'a\n\nb\n' 2 1 1 'a\nb\n'; }
@test "no-final-newline" { check 'a\nb' 1 1 1 'b'; }
@test "remove-incomplete-line" { check 'a\nb' 2 1 1 'a\n'; }
@test "crlf" { check 'a\r\nb\r\nc\r\n' 2 1 1 'a\r\nc\r\n'; }
@test "lone-cr" { check 'a\rb\nc\n' 1 1 1 'c\n'; }
@test "non-ascii" { check 'caf\xc3\xa9\nna\xc3\xafve\nzo\xc3\xab\n' 2 1 1 'caf\xc3\xa9\nzo\xc3\xab\n'; }
@test "large-count" { check 'a\nb\n' 2 2147483647 1 'a\n'; }

@test "many-lines" {
  seq 0 99999 >"$file"
  [ "$(remove_lines "$file" 2 99998)" = 99998 ]
  [ "$(cat "$file")" = "$(printf '0\n99999')" ]
}

@test "keeps-the-permissions" {
  printf 'a\nb\n' >"$file"
  chmod 640 "$file"
  [ "$(remove_lines "$file" 1 1)" = 1 ]
  [ "$(ls -l "$file" | cut -c 1-10)" = "-rw-r-----" ]
}

@test "not-found" {
  run remove_lines "$dir/missing.txt" 1 1
  [ "$status" -eq 2 ]
  [ "$output" = "remove_lines: cannot read $dir/missing.txt" ]
}

@test "bad-range" {
  printf 'a\nb\n' >"$file"
  run remove_lines "$file" 0 1
  [ "$status" -eq 2 ]
  run remove_lines "$file" 1 -1
  [ "$status" -eq 2 ]
  [ "$(cat "$file")" = "$(printf 'a\nb')" ]
}
