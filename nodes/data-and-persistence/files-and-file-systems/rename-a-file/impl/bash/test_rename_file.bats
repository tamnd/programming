#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rename-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/rename_file.sh"
  dir=$BATS_TEST_TMPDIR
}

lexists() { [[ -e $1 || -L $1 ]]; }

@test "rename" {
  printf 'alpha\n' >"$dir/a.txt"
  rename_file "$dir/a.txt" "$dir/b.txt"
  [ "$(cat "$dir/b.txt")" = alpha ]
  ! lexists "$dir/a.txt"
}

@test "replace existing" {
  printf 'alpha\n' >"$dir/a.txt"
  printf 'beta\n' >"$dir/b.txt"
  rename_file "$dir/a.txt" "$dir/b.txt"
  [ "$(cat "$dir/b.txt")" = alpha ]
  ! lexists "$dir/a.txt"
}

@test "same path" {
  printf 'alpha\n' >"$dir/a.txt"
  rename_file "$dir/a.txt" "$dir/a.txt"
  [ "$(cat "$dir/a.txt")" = alpha ]
}

@test "empty file" {
  : >"$dir/a.txt"
  printf 'beta\n' >"$dir/b.txt"
  rename_file "$dir/a.txt" "$dir/b.txt"
  [ ! -s "$dir/b.txt" ]
}

@test "other directory" {
  mkdir "$dir/sub"
  printf 'alpha\n' >"$dir/a.txt"
  rename_file "$dir/a.txt" "$dir/sub/a.txt"
  [ "$(cat "$dir/sub/a.txt")" = alpha ]
  ! lexists "$dir/a.txt"
}

@test "names with spaces, non-ASCII and a dash" {
  printf 'alpha\n' >"$dir/my notes.txt"
  rename_file "$dir/my notes.txt" "$dir/café 日本.txt"
  [ "$(cat "$dir/café 日本.txt")" = alpha ]
  cd "$dir"
  rename_file "café 日本.txt" -n
  [ "$(cat -- -n)" = alpha ]
}

@test "read-only dst" {
  printf 'alpha\n' >"$dir/a.txt"
  printf 'beta\n' >"$dir/b.txt"
  chmod 444 "$dir/b.txt"
  rename_file "$dir/a.txt" "$dir/b.txt" </dev/null
  [ "$(cat "$dir/b.txt")" = alpha ]
}

@test "errors" {
  run ! rename_file "$dir/a.txt" "$dir/b.txt"
  printf 'alpha\n' >"$dir/a.txt"
  run ! rename_file "$dir/a.txt" "$dir/no-such-dir/b.txt"
  mkdir "$dir/dir"
  run -1 rename_file "$dir/a.txt" "$dir/dir"
  [ -z "$(ls "$dir/dir")" ]
  [ "$(cat "$dir/a.txt")" = alpha ]
}

@test "link" {
  printf 'alpha\n' >"$dir/a.txt"
  ln -s a.txt "$dir/link"
  rename_file "$dir/link" "$dir/new-link"
  [ -L "$dir/new-link" ]
  ! lexists "$dir/link"
  [ "$(cat "$dir/new-link")" = alpha ]
}

@test "an open dst keeps the old data" {
  printf 'alpha\n' >"$dir/a.txt"
  printf 'beta\n' >"$dir/b.txt"
  exec 3<"$dir/b.txt"
  rename_file "$dir/a.txt" "$dir/b.txt"
  [ "$(cat <&3)" = beta ]
  exec 3<&-
  [ "$(cat "$dir/b.txt")" = alpha ]
}

@test "cross device" {
  [ -d /dev/shm ] || skip "no /dev/shm"
  [ "$(df -P /dev/shm | awk 'NR == 2 { print $NF }')" != "$(df -P "$dir" | awk 'NR == 2 { print $NF }')" ] ||
    skip "/dev/shm is on the same file system"
  printf 'alpha\n' >"$dir/a.txt"
  run -1 rename_file "$dir/a.txt" "/dev/shm/rename-file-$$.txt"
  [ -f "$dir/a.txt" ]
  [ ! -e "/dev/shm/rename-file-$$.txt" ]
}
