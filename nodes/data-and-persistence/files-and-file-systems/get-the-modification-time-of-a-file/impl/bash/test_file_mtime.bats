#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/file_mtime.sh"
  dir=$BATS_TEST_TMPDIR
}

# touch -d takes a UTC time with a part of a second, as POSIX gives it.
@test "whole-second" {
  printf 'alpha\n' >"$dir/a.txt"
  touch -d 2024-03-01T12:34:56Z "$dir/a.txt"
  [ "$(file_mtime "$dir/a.txt")" = 1709296496 ]
}

@test "sub-second" {
  printf 'alpha\n' >"$dir/a.txt"
  touch -d 2024-03-01T12:34:56.5Z "$dir/a.txt"
  [ "$(file_mtime "$dir/a.txt")" = 1709296496 ]
}

@test "epoch" {
  printf 'alpha\n' >"$dir/a.txt"
  touch -d 1970-01-01T00:00:00Z "$dir/a.txt"
  [ "$(file_mtime "$dir/a.txt")" = 0 ]
}

@test "before-1970" {
  printf 'alpha\n' >"$dir/a.txt"
  touch -d 1969-12-31T00:00:00Z "$dir/a.txt"
  [ "$(file_mtime "$dir/a.txt")" = -86400 ]
}

@test "before-1970-sub-second" {
  printf 'alpha\n' >"$dir/a.txt"
  touch -d 1969-12-31T23:59:58.5Z "$dir/a.txt"
  [ "$(file_mtime "$dir/a.txt")" = -2 ]
}

@test "after-2038" {
  printf 'alpha\n' >"$dir/a.txt"
  touch -d 2100-01-01T00:00:00Z "$dir/a.txt"
  [ "$(file_mtime "$dir/a.txt")" = 4102444800 ]
}

@test "directory" {
  mkdir "$dir/dir"
  touch -d 2024-03-01T12:34:56Z "$dir/dir"
  [ "$(file_mtime "$dir/dir")" = 1709296496 ]
}

@test "name-with-spaces" {
  printf 'alpha\n' >"$dir/my notes.txt"
  touch -d 2024-03-01T12:34:56Z "$dir/my notes.txt"
  [ "$(file_mtime "$dir/my notes.txt")" = 1709296496 ]
}

@test "missing" {
  run -1 file_mtime "$dir/a.txt"
}

@test "missing name that is a number" {
  cd "$dir"
  run -1 file_mtime 1709296496
}

@test "symbolic link" {
  printf 'alpha\n' >"$dir/a.txt"
  touch -d 2024-03-01T12:34:56Z "$dir/a.txt"
  ln -s a.txt "$dir/link"
  [ "$(file_mtime "$dir/link")" = 1709296496 ]
}
