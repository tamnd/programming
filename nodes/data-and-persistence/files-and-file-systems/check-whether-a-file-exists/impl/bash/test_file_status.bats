#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-file-exists/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/file_status.sh"
  dir=$BATS_TEST_TMPDIR
  printf 'alpha\n' >"$dir/a.txt"
  : >"$dir/empty.txt"
  mkdir "$dir/dir"
}

check() { # check PATH WANT
  local got
  file_status got "$1"
  [ "$got" = "$2" ]
}

@test "file" { check "$dir/a.txt" file; }
@test "empty file" { check "$dir/empty.txt" file; }
@test "directory" { check "$dir/dir" directory; }
@test "trailing slash" { check "$dir/dir/" directory; }
@test "missing" { check "$dir/missing.txt" missing; }
@test "missing parent" { check "$dir/no-such-dir/a.txt" missing; }
@test "through a file" { check "$dir/a.txt/b.txt" missing; }

@test "symbolic links" {
  ln -s a.txt "$dir/to-file"
  ln -s dir "$dir/to-dir"
  ln -s missing.txt "$dir/to-nothing"
  check "$dir/to-file" file
  check "$dir/to-dir" directory
  check "$dir/to-nothing" broken-link
}

@test "device" {
  [ -e /dev/null ] || skip "no /dev/null"
  check /dev/null other
}

@test "named pipe" {
  mkfifo "$dir/pipe"
  check "$dir/pipe" other
}

@test "permission error" {
  [ "$(id -u)" -ne 0 ] || skip "root can search each directory"
  mkdir "$dir/locked"
  printf 'alpha\n' >"$dir/locked/a.txt"
  chmod 000 "$dir/locked"
  run -1 file_status got "$dir/locked/a.txt"
  chmod 700 "$dir/locked"
}
