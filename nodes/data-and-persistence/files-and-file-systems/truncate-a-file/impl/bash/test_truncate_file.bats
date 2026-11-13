#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/truncate-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/truncate_file.sh"
  dir=$BATS_TEST_TMPDIR
}

# hex FILE prints the bytes of FILE as hex digits.
hex() { od -An -v -tx1 "$1" | tr -d ' \n'; }

@test "shrink" {
  printf 'alpha beta\n' >"$dir/a.txt"
  truncate_file "$dir/a.txt" 5
  [ "$(cat "$dir/a.txt")" = alpha ]
  [ "$(hex "$dir/a.txt")" = 616c706861 ]
}

@test "extend" {
  printf 'alpha' >"$dir/a.txt"
  truncate_file "$dir/a.txt" 8
  [ "$(hex "$dir/a.txt")" = 616c706861000000 ]
}

@test "same length" {
  printf 'alpha\n' >"$dir/a.txt"
  truncate_file "$dir/a.txt" 6
  [ "$(hex "$dir/a.txt")" = 616c7068610a ]
}

@test "to zero" {
  printf 'alpha\n' >"$dir/a.txt"
  truncate_file "$dir/a.txt" 0
  [ ! -s "$dir/a.txt" ]
}

@test "empty file" {
  : >"$dir/a.txt"
  truncate_file "$dir/a.txt" 3
  [ "$(hex "$dir/a.txt")" = 000000 ]
}

@test "name with spaces" {
  printf 'alpha\n' >"$dir/my notes.txt"
  truncate_file "$dir/my notes.txt" 2
  [ "$(cat "$dir/my notes.txt")" = al ]
}

@test "errors" {
  run -1 truncate_file "$dir/a.txt" 0
  [ ! -e "$dir/a.txt" ]
  mkdir "$dir/dir"
  run -1 truncate_file "$dir/dir" 0
  printf 'alpha\n' >"$dir/a.txt"
  run -1 truncate_file "$dir/a.txt" -1
  run -1 truncate_file "$dir/a.txt" 2x
  [ "$(cat "$dir/a.txt")" = alpha ]
}

@test "open in append mode" {
  printf 'alpha beta\n' >"$dir/app.log"
  exec 3>>"$dir/app.log"
  truncate_file "$dir/app.log" 0
  printf 'new\n' >&3
  exec 3>&-
  [ "$(cat "$dir/app.log")" = new ]
}
