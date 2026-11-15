#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/diff-two-texts/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/diff_two_texts.sh"
  old="$BATS_TEST_TMPDIR/old.txt"
  new="$BATS_TEST_TMPDIR/new.txt"
}

check() { # check OLD NEW WANT, each as printf %b bytes
  printf '%b' "$1" >"$old"
  printf '%b' "$2" >"$new"
  # The x keeps the final LFs, which $(...) removes.
  got=$(diff_files "$old" "$new" && printf x)
  [ "$got" = "$(printf '%bx' "$3")" ]
}

@test "simple" { check 'a\nb\nc\n' 'a\nc\nd\n' ' a\n-b\n c\n+d\n'; }
@test "change-a-line" { check 'one\ntwo\nthree\n' 'one\n2\nthree\n' ' one\n-two\n+2\n three\n'; }
@test "identical" { check 'a\nb\n' 'a\nb\n' ' a\n b\n'; }
@test "lines-of-a-poem" { check 'The cat sat\non the mat.\nIt was flat.\n' 'The cat sat\non the hat.\nIt was flat.\nThe end.\n' ' The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n'; }
@test "both-empty" { check '' '' ''; }
@test "old-empty" { check '' 'a\nb\n' '+a\n+b\n'; }
@test "new-empty" { check 'a\nb\n' '' '-a\n-b\n'; }
@test "all-different" { check 'a\nb\n' 'c\nd\n' '-a\n-b\n+c\n+d\n'; }
@test "insert-at-start" { check 'b\nc\n' 'a\nb\nc\n' '+a\n b\n c\n'; }
@test "delete-at-end" { check 'a\nb\nc\n' 'a\nb\n' ' a\n b\n-c\n'; }
@test "swap" { check 'a\nb\n' 'b\na\n' '-a\n b\n+a\n'; }
@test "repeated-lines" { check 'a\na\nb\n' 'a\nb\nb\n' ' a\n-a\n b\n+b\n'; }
@test "empty-line" { check 'a\n\nb\n' 'a\nb\n' ' a\n-\n b\n'; }
@test "no-final-newline" { check 'a\nb' 'a\nb\n' ' a\n b\n'; }
@test "crlf" { check 'a\r\nb\n' 'a\nb\n' '-a\r\n+a\n b\n'; }
@test "trailing-space" { check 'a \nb\n' 'a\nb\n' '-a \n+a\n b\n'; }
@test "normalization" { check 'caf\xc3\xa9\n' 'cafe\xcc\x81\n' '-caf\xc3\xa9\n+cafe\xcc\x81\n'; }

@test "many-lines" {
  # 100 lines, and 10 of them change. A new bash keeps the loops fast, with no
  # DEBUG trap of bats.
  seq 0 99 >"$old"
  seq 0 99 | awk '{ print ($1 % 10 == 5 ? "x" $1 : $1) }' >"$new"
  out=$(bash -c '. "$1" && diff_files "$2" "$3"' bash "$BATS_TEST_DIRNAME/diff_two_texts.sh" "$old" "$new")
  [ "$(printf '%s\n' "$out" | wc -l)" -eq 110 ]
  [ "$(printf '%s\n' "$out" | sed -n '5,7p')" = "$(printf ' 4\n-5\n+x5')" ]
  [ "$(printf '%s\n' "$out" | tail -n 1)" = " 99" ]
}

@test "not-found" {
  printf 'a\n' >"$new"
  run diff_files "$BATS_TEST_TMPDIR/missing.txt" "$new"
  [ "$status" -ne 0 ]
  [ "$output" = "diff_files: cannot read $BATS_TEST_TMPDIR/missing.txt" ]
}
