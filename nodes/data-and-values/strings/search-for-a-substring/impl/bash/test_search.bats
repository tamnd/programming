# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-for-a-substring/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/search.sh"
}

check() { # check WANT FUNCTION S SUB
  run "$2" "$3" "$4"
  [ "$status" -eq 0 ]
  [ "$output" = "$1" ]
}

# CC 81 is U+0301 COMBINING ACUTE ACCENT, and C3 A9 is U+00E9.
@test "found" { check 7 find 'Hello, world' 'world'; }
@test "first-of-two" { check 1 find 'abcabc' 'bc'; }
@test "not-found" { check -1 find 'abc' 'd'; }
@test "empty-sub" { check 0 find 'abc' ''; }
@test "empty-s" { check -1 find '' 'a'; }
@test "both-empty" { check 0 find '' ''; }
@test "longer-sub" { check -1 find 'ab' 'abc'; }
@test "overlap" { check 0 find 'aaaa' 'aa'; }
@test "case-sensitive" { check -1 find 'Hello' 'hello'; }
@test "non-ascii" { check 6 find $'naïve caf\xc3\xa9' $'caf\xc3\xa9'; }
@test "cjk" { check 3 find '日本語テキスト' 'テキ'; }
@test "astral" { check 2 find $'a\xf0\x9f\x99\x82b\xf0\x9f\x99\x82c' 'b'; }
@test "astral-sub" { check 1 find $'x\xf0\x9f\x99\x82y' $'\xf0\x9f\x99\x82y'; }
@test "combining" { check 3 find $'cafe\xcc\x81' 'e'; }
@test "precomposed" { check -1 find $'cafe\xcc\x81' $'\xc3\xa9'; }
@test "last-of-two" { check 4 find_last 'abcabc' 'bc'; }
@test "last-not-found" { check -1 find_last 'abc' 'd'; }
@test "last-empty-sub" { check 3 find_last 'abc' ''; }
@test "last-empty-s" { check 0 find_last '' ''; }
@test "last-overlap" { check 2 find_last 'aaaa' 'aa'; }
@test "last-astral" { check 3 find_last $'\xf0\x9f\x99\x82a\xf0\x9f\x99\x82a' 'a'; }
@test "last-non-ascii" { check 8 find_last $'caf\xc3\xa9 caf\xc3\xa9' $'\xc3\xa9'; }
