#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/lock-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  command -v flock >/dev/null || skip "the flock command of util-linux is not installed"
  source "$BATS_TEST_DIRNAME/lock_file.sh"
  dir=$BATS_TEST_TMPDIR/tree
  mkdir -p "$dir"
  printf 'alpha\n' > "$dir/data.txt"
}

check() {
  run -0 try_lock "$dir/data.txt" "$1" "$2"
  [ "$output" = "$3" ]
  [ "$(cat "$dir/data.txt")" = alpha ]
}

@test "no-lock-exclusive" { check none exclusive true; }
@test "no-lock-shared" { check none shared true; }
@test "exclusive-with-exclusive" { check exclusive exclusive false; }
@test "shared-with-shared" { check shared shared true; }
@test "shared-with-exclusive" { check shared exclusive false; }
@test "exclusive-with-shared" { check exclusive shared false; }

@test "unlock-frees-the-lock" {
  exec {a}<"$dir/data.txt" {b}<"$dir/data.txt"
  try_lock_file "$a" exclusive
  ! try_lock_file "$b" exclusive
  unlock_file "$a"
  try_lock_file "$b" exclusive
  exec {a}<&- {b}<&-
}

@test "missing-file" {
  run -1 try_lock "$dir/missing.txt" none exclusive
  [ ! -e "$dir/missing.txt" ]
}
