#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-word-frequencies/.

setup() {
  source "$BATS_TEST_DIRNAME/top_words.sh"
}

# check TEXT N WANT compares the output with WANT, which has one "word count"
# line for each word.
check() {
  top_words "$1" "$2" >"$BATS_TEST_TMPDIR/got"
  printf '%s' "$3" >"$BATS_TEST_TMPDIR/want"
  cmp "$BATS_TEST_TMPDIR/got" "$BATS_TEST_TMPDIR/want"
}

@test "sentence" { check $'the cat and the hat and the bat' 3 $'the 3\nand 2\nbat 1\n'; }
@test "all-words" { check $'one two two three three three' 10 $'three 3\ntwo 2\none 1\n'; }
@test "punctuation" { check $'Stop. Stop, stop! Go?' 2 $'stop 3\ngo 1\n'; }
@test "lines" { check $'red green\nblue red\ngreen red\n' 2 $'red 3\ngreen 2\n'; }
@test "empty" { check $'' 3 $''; }
@test "no-words" { check $' ... ,!? \n' 3 $''; }
@test "case" { check $'Word word WORD wOrD' 1 $'word 4\n'; }
@test "ties" { check $'pear fig apple fig pear apple' 3 $'apple 2\nfig 2\npear 2\n'; }
@test "fold-before-sort" { check $'Zebra apple' 2 $'apple 1\nzebra 1\n'; }
@test "n-zero" { check $'a b c' 0 $''; }
@test "n-above-count" { check $'a b a' 10 $'a 2\nb 1\n'; }
@test "non-ascii" { check $'café naïve café 日本語 naïve café' 3 $'café 3\nnaïve 2\n日本語 1\n'; }
@test "code-point-order" { check $'f é e' 3 $'e 1\nf 1\né 1\n'; }
@test "apostrophe" { check $'don\'t stop, don\'t' 3 $'don 2\nt 2\nstop 1\n'; }
@test "digits" { check $'route 66 and route 66a 66' 3 $'66 2\nroute 2\n66a 1\n'; }
@test "separators" { check $'tab\there\r\nunder_score-dash' 10 $'dash 1\nhere 1\nscore 1\ntab 1\nunder 1\n'; }
