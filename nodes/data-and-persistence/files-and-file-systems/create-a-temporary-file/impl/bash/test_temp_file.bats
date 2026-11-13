#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-temporary-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/temp_file.sh"
  dir=$BATS_TEST_TMPDIR/tree
  mkdir -p "$dir/sub"
}

# check DIR PREFIX DATA: use_temp_file prints DATA, and no file is left.
# The x keeps the newlines at the end, which $( ) removes.
check() {
  local out
  out=$(use_temp_file "$1" "$2" "$3" && printf x) || return 1
  [ "${out%x}" = "$3" ] || return 1
  [ "$(ls -A "$dir")" = sub ] || return 1
  [ -z "$(ls -A "$dir/sub")" ]
}

# mode PATH prints the permission bits, with GNU or BSD stat.
mode() {
  stat -c '%a' -- "$1" 2>/dev/null || stat -f '%Lp' -- "$1"
}

@test "one-line" { check "$dir" tmp- $'alpha\n'; }
@test "many-lines" { check "$dir" report- $'alpha\nbeta\ngamma\n'; }
@test "in-subdirectory" { check "$dir/sub" tmp- $'alpha\n'; }
@test "empty-prefix" { check "$dir" "" $'alpha\n'; }
@test "empty-data" { check "$dir" tmp- ""; }
@test "non-ascii-data" { check "$dir" tmp- $'café 日本\n'; }
@test "prefix-with-spaces" { check "$dir" "my temp " $'alpha\n'; }
@test "non-ascii-prefix" { check "$dir" café- $'alpha\n'; }

@test "make-temp-file" {
  local prefix path name
  for prefix in tmp- "" "my temp " café-; do
    path=$(make_temp_file "$dir" "$prefix")
    [ "${path%/*}" = "$dir" ]
    name=${path##*/}
    [[ $name == "$prefix"* ]]
    [ "${#name}" -ge $((${#prefix} + 6)) ]
    [ -f "$path" ] && [ ! -L "$path" ] && [ ! -s "$path" ]
    [ "$(mode "$path")" = 600 ]
  done
}

@test "two-calls-give-two-files" {
  local a b
  a=$(make_temp_file "$dir" tmp-)
  b=$(make_temp_file "$dir" tmp-)
  [ "$a" != "$b" ]
  [ "$(ls -A "$dir" | wc -l | tr -d ' ')" = 3 ]
}

@test "missing-directory" {
  run -1 use_temp_file "$dir/nodir" tmp- $'alpha\n'
}

@test "prefix-with-slash" {
  run -2 use_temp_file "$dir" sub/tmp- $'alpha\n'
  [ -z "$(ls -A "$dir/sub")" ]
}
