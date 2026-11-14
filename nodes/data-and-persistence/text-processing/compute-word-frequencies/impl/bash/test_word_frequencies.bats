#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-word-frequencies/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/word_frequencies.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES N WANT
  printf '%b' "$1" >"$file"
  run word_frequencies "$file" "$2"
  [ "$status" -eq 0 ]
  [ "$output" = "$(printf '%b' "$3")" ]
}

@test "simple" { check 'the cat and the hat\n' 2 'the 2\nand 1'; }
@test "case" { check 'The THE the tHe\n' 5 'the 4'; }
@test "punctuation" { check "it's well-dressed, isn't it?\n" 10 'it 2\ndressed 1\nisn 1\ns 1\nt 1\nwell 1'; }
@test "empty" { check '' 3 ''; }
@test "no-letters" { check '123 ... 42\n-- !!\n' 3 ''; }
@test "ties" { check 'b a c b a c\n' 2 'a 2\nb 2'; }
@test "digits" { check 'abc123def 42 x9\n' 10 'abc 1\ndef 1\nx 1'; }
@test "n-zero" { check 'the cat and the hat\n' 0 ''; }
@test "n-above-count" { check 'x y x\n' 10 'x 2\ny 1'; }
@test "crlf" { check 'one two\r\ntwo\r\n' 5 'two 2\none 1'; }
@test "non-ascii" { check 'caf\xc3\xa9 na\xc3\xafve cafe\n' 10 'caf 1\ncafe 1\nna 1\nve 1'; }
@test "leading-separators" { check '  ...hello, world. Hello\n' 5 'hello 2\nworld 1'; }
@test "no-final-newline" { check 'dog cat dog' 5 'dog 2\ncat 1'; }
@test "not-utf8" { check 'ok \xff\xfe ok\n' 5 'ok 2'; }

@test "many-lines" {
  yes 'a b a' | head -n 100000 >"$file"
  run word_frequencies "$file" 3
  [ "$output" = "$(printf 'a 200000\nb 100000')" ]
}

@test "not-found" {
  run word_frequencies "$BATS_TEST_TMPDIR/missing.txt" 3
  [ "$status" -ne 0 ]
}
