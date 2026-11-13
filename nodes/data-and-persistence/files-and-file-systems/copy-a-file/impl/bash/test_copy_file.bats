#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/copy-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/copy_file.sh"
  dir=$BATS_TEST_TMPDIR
}

@test "copy" {
  printf 'alpha\n' >"$dir/a.txt"
  copy_file "$dir/a.txt" "$dir/b.txt"
  [ "$(cat "$dir/b.txt")" = alpha ]
  [ "$(cat "$dir/a.txt")" = alpha ]
}

@test "replace existing" {
  printf 'alpha\n' >"$dir/a.txt"
  printf 'beta, a longer text\n' >"$dir/b.txt"
  copy_file "$dir/a.txt" "$dir/b.txt"
  cmp "$dir/a.txt" "$dir/b.txt"
}

@test "empty file" {
  : >"$dir/a.txt"
  printf 'beta\n' >"$dir/b.txt"
  copy_file "$dir/a.txt" "$dir/b.txt"
  [ ! -s "$dir/b.txt" ]
}

@test "other directory" {
  mkdir "$dir/sub"
  printf 'alpha\n' >"$dir/a.txt"
  copy_file "$dir/a.txt" "$dir/sub/a.txt"
  [ "$(cat "$dir/sub/a.txt")" = alpha ]
}

@test "name with spaces" {
  printf 'alpha\n' >"$dir/my notes.txt"
  copy_file "$dir/my notes.txt" "$dir/copy of my notes.txt"
  [ "$(cat "$dir/copy of my notes.txt")" = alpha ]
}

@test "non-ascii name" {
  printf 'alpha\n' >"$dir/a.txt"
  copy_file "$dir/a.txt" "$dir/café 日本.txt"
  [ "$(cat "$dir/café 日本.txt")" = alpha ]
}

@test "binary data" {
  printf '\0\r\n\303\277\0' >"$dir/a.bin"
  copy_file "$dir/a.bin" "$dir/b.bin"
  cmp "$dir/a.bin" "$dir/b.bin"
  [ "$(wc -c <"$dir/b.bin" | tr -d ' ')" = 6 ]
}

@test "missing src or dst directory" {
  run -1 copy_file "$dir/a.txt" "$dir/b.txt"
  printf 'alpha\n' >"$dir/a.txt"
  run -1 copy_file "$dir/a.txt" "$dir/no-such-dir/b.txt"
}

@test "directory at src or dst" {
  mkdir "$dir/dir"
  printf 'alpha\n' >"$dir/a.txt"
  run -1 copy_file "$dir/dir" "$dir/b.txt"
  run -1 copy_file "$dir/a.txt" "$dir/dir"
  [ -z "$(ls -A "$dir/dir")" ]
  [ ! -e "$dir/b.txt" ]
}

@test "same file" {
  printf 'alpha\n' >"$dir/a.txt"
  ln "$dir/a.txt" "$dir/b.txt"
  run -1 copy_file "$dir/a.txt" "$dir/a.txt"
  run -1 copy_file "$dir/a.txt" "$dir/b.txt"
  [ "$(cat "$dir/a.txt")" = alpha ]
}

@test "permission bits" {
  printf 'echo alpha\n' >"$dir/a.sh"
  printf 'beta\n' >"$dir/c.sh"
  chmod 750 "$dir/a.sh"
  chmod 640 "$dir/c.sh"
  copy_file "$dir/a.sh" "$dir/b.sh"
  copy_file "$dir/a.sh" "$dir/c.sh"
  [[ $(ls -l "$dir/b.sh") == -rwxr-x---* ]]
  [[ $(ls -l "$dir/c.sh") == -rwxr-x---* ]]
}

@test "large file" {
  head -c 5000003 /dev/urandom >"$dir/big.bin"
  copy_file "$dir/big.bin" "$dir/copy.bin"
  cmp "$dir/big.bin" "$dir/copy.bin"
}

@test "link at src" {
  printf 'alpha\n' >"$dir/target.txt"
  ln -s target.txt "$dir/link.txt"
  copy_file "$dir/link.txt" "$dir/b.txt"
  [ ! -L "$dir/b.txt" ]
  [ "$(cat "$dir/b.txt")" = alpha ]
}
