#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-symbolic-link/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/make_symlink.sh"
  dir=$BATS_TEST_TMPDIR
  printf 'alpha\n' >"$dir/a.txt"
  mkdir "$dir/sub"
  printf 'beta\n' >"$dir/sub/b.txt"
}

@test "file-target" {
  [ "$(make_symlink a.txt "$dir/link")" = a.txt ]
  [ -L "$dir/link" ]
}

@test "directory-target" {
  [ "$(make_symlink sub "$dir/link")" = sub ]
  [ -L "$dir/link" ]
}

@test "link-in-subdirectory" {
  [ "$(make_symlink b.txt "$dir/sub/link")" = b.txt ]
}

@test "dangling-target" {
  [ "$(make_symlink missing.txt "$dir/link")" = missing.txt ]
  [ -L "$dir/link" ]
  [ ! -e "$dir/link" ]
}

@test "relative-target" {
  [ "$(make_symlink a.txt "$dir/sub/link")" = a.txt ]
  [ ! -e "$dir/sub/link" ]
}

@test "parent-target" {
  [ "$(make_symlink ../a.txt "$dir/sub/link")" = ../a.txt ]
  [ "$(cat "$dir/sub/link")" = alpha ]
}

@test "name-with-spaces" {
  [ "$(make_symlink 'my notes.txt' "$dir/my link")" = 'my notes.txt' ]
}

@test "non-ascii-name" {
  [ "$(make_symlink 'café 日本.txt' "$dir/lien 日本")" = 'café 日本.txt' ]
}

@test "a file exists" {
  run -1 make_symlink b.txt "$dir/a.txt"
  [ ! -L "$dir/a.txt" ]
}

@test "a directory exists, and no link is made in it" {
  run -1 make_symlink a.txt "$dir/sub"
  [ ! -e "$dir/sub/a.txt" ]
  [ ! -L "$dir/sub/a.txt" ]
}

@test "a dangling link exists" {
  ln -s missing.txt "$dir/link"
  run -1 make_symlink a.txt "$dir/link"
  [ "$(readlink "$dir/link")" = missing.txt ]
}

@test "missing directory" {
  run -1 make_symlink a.txt "$dir/nodir/link"
}

@test "absolute target" {
  [ "$(make_symlink "$dir/a.txt" "$dir/link")" = "$dir/a.txt" ]
}
