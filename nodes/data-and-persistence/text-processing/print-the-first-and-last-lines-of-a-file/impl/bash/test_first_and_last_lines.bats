#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/first_and_last_lines.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

# joined ARRAY prints each item of the array named ARRAY with an LF after it,
# and then an x, which keeps the final LFs that $(...) removes.
joined() {
  local -n joined_items=$1
  local item
  for item in "${joined_items[@]}"; do printf '%s\n' "$item"; done
  printf x
}

check() { # check BYTES COUNT FIRST LAST
  printf '%b' "$1" >"$file"
  local -a first last
  first_and_last_lines "$file" "$2" first last
  [ "$(joined first)" = "$(printf '%bx' "$3")" ] || return 1
  [ "$(joined last)" = "$(printf '%bx' "$4")" ]
}

@test "two" { check 'a\nb\nc\nd\ne\n' 2 'a\nb\n' 'd\ne\n'; }
@test "one" { check 'first\nmiddle\nlast\n' 1 'first\n' 'last\n'; }
@test "ten" { check "$(seq 1 12)\n" 10 "$(seq 1 10)\n" "$(seq 3 12)\n"; }
@test "empty" { check '' 2 '' ''; }
@test "zero" { check 'a\nb\n' 0 '' ''; }
@test "short" { check 'a\nb\n' 5 'a\nb\n' 'a\nb\n'; }
@test "exact" { check 'a\nb\n' 2 'a\nb\n' 'a\nb\n'; }
@test "overlap" { check 'a\nb\nc\n' 2 'a\nb\n' 'b\nc\n'; }
@test "one-line" { check 'a\n' 1 'a\n' 'a\n'; }
@test "empty-lines" { check '\n\na\n\n' 1 '\n' '\n'; }
@test "only-newline" { check '\n' 1 '\n' '\n'; }
@test "no-final-newline" { check 'a\nb\nc' 1 'a\n' 'c\n'; }
@test "crlf" { check 'a\r\nb\r\n' 1 'a\r\n' 'b\r\n'; }
@test "non-ascii" { check 'caf\xc3\xa9\nna\xc3\xafve\nzo\xc3\xab\n' 1 'caf\xc3\xa9\n' 'zo\xc3\xab\n'; }

@test "many-lines" {
  seq 0 99999 >"$file"
  local -a first last
  first_and_last_lines "$file" 3 first last
  [ "${first[*]}" = "0 1 2" ]
  [ "${last[*]}" = "99997 99998 99999" ]
}

@test "not-found" {
  local -a first last
  run first_and_last_lines "$BATS_TEST_TMPDIR/missing.txt" 1 first last
  [ "$status" -ne 0 ]
  [ "$output" = "first_and_last_lines: cannot read $BATS_TEST_TMPDIR/missing.txt" ]
}
