#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/watch-a-file-for-changes/.

bats_require_minimum_version 1.5.0

BASE_MTIME=1700000000

setup() {
  source "$BATS_TEST_DIRNAME/watch_file.sh"
  path=$BATS_TEST_TMPDIR/watched.txt
  rm -rf "$path" "$path.tmp"
}

# set_mtime SECONDS FILE: GNU touch takes -d @SECONDS; BSD touch takes -t.
set_mtime() {
  touch -d "@$1" "$2" 2>/dev/null || touch -t "$(date -r "$1" +%Y%m%d%H%M.%S)" "$2"
}

# apply_step STEP NUMBER: makes the change of one step of a case.
apply_step() {
  local kind=${1%%:*} text=
  [[ $1 == *:* ]] && text=${1#*:}
  case $kind in
    create | write) printf '%s' "$text" > "$path" ;;
    append) [ -f "$path" ] && printf '%s' "$text" >> "$path" ;;
    replace) printf '%s' "$text" > "$path.tmp" && mv -f "$path.tmp" "$path" ;;
    recreate) rm "$path" && printf '%s' "$text" > "$path" ;;
    delete) rm "$path" ;;
    mkdir) mkdir "$path" ;;
    touch | none) ;;
    *) return 1 ;;
  esac || return 1
  if [ "$kind" != none ] && [ -f "$path" ]; then
    set_mtime $((BASE_MTIME + $2)) "$path"
  fi
}

# check EXPECT STEP...: the results of the polls, one for each step, joined by spaces.
check() {
  local expect=$1 got=() i=0 step
  shift
  watch_init "$path" || return 1
  for step in "$@"; do
    i=$((i + 1))
    apply_step "$step" "$i" || return 1
    watch_poll || return 1
    got+=("$WATCH_CHANGE")
  done
  [ "${got[*]}" = "$expect" ]
}

@test "create" { check created $'create:hello\n'; }
@test "create-then-write" { check "created modified" $'create:a\n' $'write:a much longer line\n'; }
@test "create-then-delete" { check "created deleted" $'create:a\n' delete; }
@test "append" { check "created modified modified" $'create:a\n' $'append:b\n' $'append:c\n'; }
@test "no-change" { check "created none none" $'create:a\n' none none; }
@test "same-size" { check "created modified" $'create:abc\n' $'write:xyz\n'; }
@test "touch-only" { check "created modified" $'create:a\n' touch; }
@test "replace-by-rename" { check "created modified" $'create:old\n' $'replace:new\n'; }
@test "delete-and-create" { check "created modified" $'create:a\n' $'recreate:b\n'; }
@test "never-created" { check none none; }

@test "a directory is an error" {
  watch_init "$path"
  mkdir "$path"
  run ! watch_poll
}

@test "the same size and the same mtime hide a change" {
  printf 'abc\n' > "$path"
  set_mtime 5 "$path"
  watch_init "$path"
  printf 'xyz\n' > "$path"
  set_mtime 5 "$path"
  watch_poll
  [ "$WATCH_CHANGE" = none ]
  printf 'xyz!\n' > "$path"
  watch_poll
  [ "$WATCH_CHANGE" = modified ]
}
