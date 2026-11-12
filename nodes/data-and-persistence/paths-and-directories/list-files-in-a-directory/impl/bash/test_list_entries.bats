#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/list-files-in-a-directory/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/list_entries.sh"
  root="$BATS_TEST_TMPDIR/tree"
  mkdir "$root"
}

make_tree() { # make_tree ENTRY...
  local entry
  for entry in "$@"; do
    if [[ $entry == *" -> "* ]]; then
      ln -s "${entry#* -> }" "$root/${entry%% -> *}"
    elif [[ $entry == */ ]]; then
      mkdir "$root/$entry"
    else
      : >"$root/$entry"
    fi
  done
}

check() { # check FLAGS WANT, after make_tree
  run list_entries $1 "$root"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

nl=$'\n'
tree=(docs/ docs/guide.md docs/img/ docs/img/logo.png readme.md)

@test "flat" { make_tree b.txt a.txt c.txt; check "" "a.txt${nl}b.txt${nl}c.txt"; }
@test "nested" { make_tree "${tree[@]}"; check -r "docs/${nl}docs/guide.md${nl}docs/img/${nl}docs/img/logo.png${nl}readme.md"; }
@test "nested-shallow" { make_tree "${tree[@]}"; check "" "docs/${nl}readme.md"; }
@test "empty" { check -r ""; }
@test "empty-subdir" { make_tree empty/; check -r "empty/"; }
@test "hidden" { make_tree .env .git/ .git/config main.go; check -r ".env${nl}.git/${nl}.git/config${nl}main.go"; }
@test "case-order" { make_tree a.txt B.txt c.txt; check "" "B.txt${nl}a.txt${nl}c.txt"; }
@test "dot-before-slash" { make_tree a/ a/x a.txt a-b; check -r "a-b${nl}a.txt${nl}a/${nl}a/x"; }
@test "symlink-to-dir" { make_tree real/ real/f "link -> real"; check -r "link${nl}real/${nl}real/f"; }
@test "symlink-to-file" { make_tree f "g -> f"; check -r "f${nl}g"; }
@test "broken-symlink" { make_tree "dangling -> nowhere"; check -r "dangling"; }
@test "symlink-loop" { make_tree d/ "d/up -> .."; check -r "d/${nl}d/up"; }
@test "spaces" { make_tree "my file.txt" "two  spaces/" "two  spaces/x"; check -r "my file.txt${nl}two  spaces/${nl}two  spaces/x"; }
@test "non-ascii" { make_tree été.txt zebra.txt apple.txt; check "" "apple.txt${nl}zebra.txt${nl}été.txt"; }

@test "missing" {
  run --separate-stderr list_entries -r "$root/missing"
  [ "$status" -eq 1 ]
  [ -z "$output" ]
}

@test "not-a-directory" {
  make_tree notes.txt
  run --separate-stderr list_entries -r "$root/notes.txt"
  [ "$status" -eq 1 ]
  [ -z "$output" ]
}

@test "shell options unchanged" {
  shopt -u dotglob nullglob
  list_entries "$root" >/dev/null
  ! shopt -q dotglob
  ! shopt -q nullglob
}
