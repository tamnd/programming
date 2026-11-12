# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-a-substring/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/replace.sh"
}

check() { # check WANT FUNCTION S OLD NEW
  run "$2" "$3" "$4" "$5"
  [ "$status" -eq 0 ]
  [ "$output" = "$1" ]
}

# CC 81 is U+0301 COMBINING ACUTE ACCENT, and C3 A9 is U+00E9.
@test "all" { check 'a+b+c' replace 'a-b-c' '-' '+'; }
@test "word" { check 'Hello, there' replace 'Hello, world' 'world' 'there'; }
@test "none" { check 'abc' replace 'abc' 'x' 'y'; }
@test "empty-s" { check '' replace '' 'a' 'b'; }
@test "delete" { check 'abc' replace 'a-b-c' '-' ''; }
@test "new-contains-old" { check 'aXXXa' replace 'aXa' 'X' 'XXX'; }
@test "overlap" { check 'bb' replace 'aaaa' 'aa' 'b'; }
@test "overlap-odd" { check 'ba' replace 'aaa' 'aa' 'b'; }
@test "case-sensitive" { check 'Hello bye' replace 'Hello hello' 'hello' 'bye'; }
@test "non-ascii" { check 'naïve cafe' replace $'naïve caf\xc3\xa9' $'\xc3\xa9' 'e'; }
@test "astral" { check 'a!b!' replace $'a\xf0\x9f\x99\x82b\xf0\x9f\x99\x82' $'\xf0\x9f\x99\x82' '!'; }
@test "combining" { check $'cafE\xcc\x81' replace $'cafe\xcc\x81' 'e' 'E'; }
@test "precomposed" { check $'caf\xc3\xa9' replace $'caf\xc3\xa9' 'e' 'E'; }
@test "dot" { check 'a-b-c' replace 'a.b.c' '.' '-'; }
@test "dollar" { check '$&!' replace 'price' 'price' '$&!'; }
@test "ampersand" { check 'a&b' replace 'a-b' '-' '&'; }
@test "backslash" { check $'a\\b' replace 'a/b' '/' $'\\'; }
@test "first-only" { check 'a+b-c' replace_first 'a-b-c' '-' '+'; }
@test "first-none" { check 'abc' replace_first 'abc' 'x' 'y'; }
@test "first-overlap" { check 'ba' replace_first 'aaa' 'aa' 'b'; }
@test "first-astral" { check $'x\xf0\x9f\x99\x82' replace_first $'\xf0\x9f\x99\x82\xf0\x9f\x99\x82' $'\xf0\x9f\x99\x82' 'x'; }
@test "first-dollar" { check 'a$&b' replace_first 'a.b' '.' '$&'; }
