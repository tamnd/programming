#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-size-of-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/file_size.sh"
  dir=$BATS_TEST_TMPDIR
}

@test "text" {
  printf 'alpha\n' >"$dir/a.txt"
  [ "$(file_size "$dir/a.txt")" = 6 ]
}

@test "non-ascii-text" {
  printf 'caf\303\251\n' >"$dir/a.txt"
  [ "$(file_size "$dir/a.txt")" = 6 ]
}

@test "empty-file" {
  : >"$dir/a.txt"
  [ "$(file_size "$dir/a.txt")" = 0 ]
}

@test "binary-data" {
  printf '\000\r\n\303\277\000' >"$dir/a.bin"
  [ "$(file_size "$dir/a.bin")" = 6 ]
}

@test "crlf-line-ends" {
  printf 'a\r\nb\r\n' >"$dir/a.txt"
  [ "$(file_size "$dir/a.txt")" = 6 ]
}

@test "name-with-spaces" {
  printf 'alpha\n' >"$dir/my notes.txt"
  [ "$(file_size "$dir/my notes.txt")" = 6 ]
}

@test "non-ascii-name" {
  printf 'alpha\n' >"$dir/café 日本.txt"
  [ "$(file_size "$dir/café 日本.txt")" = 6 ]
}

@test "missing" {
  run -1 file_size "$dir/a.txt"
}

@test "directory" {
  mkdir "$dir/dir"
  run -1 file_size "$dir/dir"
}

@test "symbolic link" {
  printf 'alpha beta\n' >"$dir/a.txt"
  ln -s a.txt "$dir/link"
  [ "$(file_size "$dir/link")" = 11 ]
}

@test "sparse file of 5 GiB" {
  dd if=/dev/null of="$dir/sparse.bin" bs=1 seek=5368709120 2>/dev/null
  [ "$(file_size "$dir/sparse.bin")" = 5368709120 ]
}
