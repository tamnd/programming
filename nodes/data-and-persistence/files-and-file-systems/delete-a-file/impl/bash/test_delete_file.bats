#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/delete-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/delete_file.sh"
  dir=$BATS_TEST_TMPDIR
}

lexists() { [[ -e $1 || -L $1 ]]; }

check() { # check PATH WANT
  local got
  delete_file got "$1"
  [ "$got" = "$2" ]
  ! lexists "$1"
}

@test "delete" { printf 'alpha\n' >"$dir/a.txt"; check "$dir/a.txt" true; }
@test "missing" { check "$dir/a.txt" false; }
@test "empty file" { : >"$dir/a.txt"; check "$dir/a.txt" true; }
@test "missing parent" { check "$dir/no-such-dir/a.txt" false; }
@test "name with spaces" { printf 'alpha\n' >"$dir/my notes.txt"; check "$dir/my notes.txt" true; }
@test "non-ASCII name" { printf 'alpha\n' >"$dir/café 日本.txt"; check "$dir/café 日本.txt" true; }
@test "name that starts with a dash" { printf 'alpha\n' >"$dir/-n"; cd "$dir"; check -n true; }

@test "through a file" {
  printf 'alpha\n' >"$dir/a.txt"
  check "$dir/a.txt/b.txt" false
  [ -f "$dir/a.txt" ]
}

@test "directory" {
  mkdir "$dir/dir"
  run -1 delete_file got "$dir/dir"
  [ -d "$dir/dir" ]
}

@test "links" {
  printf 'alpha\n' >"$dir/a.txt"
  mkdir "$dir/dir"
  ln -s a.txt "$dir/to-file"
  ln -s dir "$dir/to-dir"
  ln -s missing.txt "$dir/broken"
  check "$dir/to-file" true
  check "$dir/to-dir" true
  check "$dir/broken" true
  [ "$(cat "$dir/a.txt")" = alpha ]
  [ -d "$dir/dir" ]
}

@test "read-only file" {
  printf 'alpha\n' >"$dir/a.txt"
  chmod 444 "$dir/a.txt"
  check "$dir/a.txt" true
}

@test "directory with no write permission" {
  [ "$(id -u)" -ne 0 ] || skip "root can write to each directory"
  mkdir "$dir/locked"
  printf 'alpha\n' >"$dir/locked/a.txt"
  chmod 500 "$dir/locked"
  run -1 delete_file got "$dir/locked/a.txt"
  chmod 700 "$dir/locked"
  [ -f "$dir/locked/a.txt" ]
}

@test "open file" {
  printf 'alpha\n' >"$dir/a.txt"
  exec 3<"$dir/a.txt"
  check "$dir/a.txt" true
  [ "$(cat <&3)" = alpha ] # on Unix the data stays until the close
  exec 3<&-
}
