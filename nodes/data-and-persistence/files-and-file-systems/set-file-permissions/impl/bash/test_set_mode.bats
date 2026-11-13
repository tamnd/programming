#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/set-file-permissions/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/set_mode.sh"
  dir=$BATS_TEST_TMPDIR
  printf 'alpha\n' >"$dir/a.txt"
  chmod 644 "$dir/a.txt"
}

teardown() {
  chmod -R u+rwx "$dir" 2>/dev/null || true
}

@test "owner-read-write-group-read" {
  [ "$(set_mode "$dir/a.txt" 640)" = 640 ]
}

@test "owner-only" {
  [ "$(set_mode "$dir/a.txt" 600)" = 600 ]
}

@test "executable" {
  [ "$(set_mode "$dir/a.txt" 755)" = 755 ]
}

@test "same-mode" {
  [ "$(set_mode "$dir/a.txt" 644)" = 644 ]
}

@test "no-permissions" {
  [ "$(set_mode "$dir/a.txt" 000)" = 000 ]
}

@test "leading-zero" {
  [ "$(set_mode "$dir/a.txt" 0640)" = 640 ]
}

@test "set-user-id" {
  [ "$(set_mode "$dir/a.txt" 4755)" = 4755 ]
}

@test "name-with-spaces" {
  printf 'alpha\n' >"$dir/my notes.txt"
  [ "$(set_mode "$dir/my notes.txt" 640)" = 640 ]
}

@test "non-ascii-name" {
  printf 'alpha\n' >"$dir/café 日本.txt"
  [ "$(set_mode "$dir/café 日本.txt" 640)" = 640 ]
}

@test "invalid modes fail with status 2 and change nothing" {
  for m in 648 64 10640 u+x '' $'640\n' +640; do
    run -2 set_mode "$dir/a.txt" "$m"
  done
  [ "$(set_mode "$dir/a.txt" 644)" = 644 ]
}

@test "missing file fails with status 1" {
  run -1 set_mode "$dir/missing.txt" 640
}

@test "a symbolic link changes the target" {
  ln -s a.txt "$dir/link"
  [ "$(set_mode "$dir/link" 600)" = 600 ]
  [ "$(set_mode "$dir/a.txt" 600)" = 600 ]
}

@test "directory" {
  mkdir "$dir/d"
  [ "$(set_mode "$dir/d" 700)" = 700 ]
}
