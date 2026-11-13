#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sync-a-file-to-disk/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/write_synced.sh"
  dir=$BATS_TEST_TMPDIR/tree
  mkdir -p "$dir"
}

# check TEXT [BEFORE]: after write_synced, out.txt holds TEXT, and it is the
# only file. The x keeps the newlines at the end, which $( ) removes.
check() {
  local out
  if [ $# -gt 1 ]; then printf '%s' "$2" > "$dir/out.txt"; fi
  write_synced "$dir/out.txt" "$1" || return 1
  out=$(cat "$dir/out.txt" && printf x) || return 1
  [ "${out%x}" = "$1" ] || return 1
  [ "$(ls -A "$dir")" = out.txt ]
}

@test "new-file" { check $'alpha\nbeta\n'; }
@test "replace-longer" { check $'short\n' $'a much longer old line\n'; }
@test "many-lines" { check $'one\ntwo\nthree\nfour\n'; }
@test "empty-replace" { check '' $'old\n'; }
@test "empty-new" { check ''; }
@test "no-final-newline" { check 'last line'; }
@test "non-ascii" { check $'naïve café\n日本語\n🙂\n'; }
@test "crlf" { check $'one\r\ntwo\r\n'; }

@test "missing-directory" {
  run -1 write_synced "$dir/missing/out.txt" $'x\n'
  [ -z "$(ls -A "$dir")" ]
}

@test "onto-directory" {
  mkdir "$dir/out.txt"
  run -1 write_synced "$dir/out.txt" $'x\n'
  [ -d "$dir/out.txt" ]
}
