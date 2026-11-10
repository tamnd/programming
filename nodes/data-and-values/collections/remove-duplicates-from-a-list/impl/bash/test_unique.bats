#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/unique.sh"
}

lines() { # lines ITEM...: prints each item on a line, and nothing for no items
  if (($# > 0)); then printf '%s\n' "$@"; fi
}

check() { # check FUNCTION GIVEN... -- WANT...
  local fn=$1 got want
  shift
  local -a given=()
  while [ "$1" != -- ]; do given+=("$1"); shift; done
  shift
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(lines "${given[@]}" | "$fn"; printf x)
  want=$(lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

E=$(printf '\303\251')   # U+00E9, one code point
D=$(printf 'e\314\201')  # e and U+0301, two code points
TAB=$(printf '\t')

@test "repeats" { check unique apple pear apple fig pear -- apple pear fig; }
@test "words" { check unique to be or not to be -- to be or not; }
@test "empty" { check unique --; }
@test "single" { check unique a -- a; }
@test "all-same" { check unique x x x -- x; }
@test "no-duplicates" { check unique c a b -- c a b; }
@test "not-adjacent" { check unique a b a b -- a b; }
@test "adjacent" { check unique a a b b a -- a b; }
@test "empty-string" { check unique "" a "" b -- "" a b; }
@test "case" { check unique a A a -- a A; }
@test "normalization" { check unique "$E" "$D" "$E" -- "$E" "$D"; }
@test "whitespace" { check unique " a" a "a " " a" -- " a" a "a "; }
@test "number-like" { check unique 1 01 1.0 1 -- 1 01 1.0; }
@test "non-ascii" { check unique 日本 🙂 日本 🙂 ü -- 日本 🙂 ü; }

# The cases of cases/by-key.jsonl. Each user is NAME, a tab and EMAIL.
@test "by-key case-insensitive" {
  check unique_by_email "Ada${TAB}ada@example.com" "Bob${TAB}bob@example.com" "Ada L.${TAB}ADA@Example.com" -- \
    "Ada${TAB}ada@example.com" "Bob${TAB}bob@example.com"
}
@test "by-key empty" { check unique_by_email --; }
@test "by-key same-name-other-email" {
  check unique_by_email "Sam${TAB}sam@a.example" "Sam${TAB}sam@b.example" -- "Sam${TAB}sam@a.example" "Sam${TAB}sam@b.example"
}
@test "by-key first-record-wins" {
  check unique_by_email "Old${TAB}x@example.com" "New${TAB}X@EXAMPLE.COM" -- "Old${TAB}x@example.com"
}
@test "by-key dots-differ" {
  check unique_by_email "Al${TAB}a.b@example.com" "Bo${TAB}ab@example.com" -- "Al${TAB}a.b@example.com" "Bo${TAB}ab@example.com"
}
@test "by-key not-adjacent" {
  check unique_by_email "A${TAB}a@example.com" "B${TAB}b@example.com" "C${TAB}A@example.com" "D${TAB}B@EXAMPLE.com" -- \
    "A${TAB}a@example.com" "B${TAB}b@example.com"
}
