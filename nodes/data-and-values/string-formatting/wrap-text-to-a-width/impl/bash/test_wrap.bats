#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wrap-text-to-a-width/.

setup() {
  source "$BATS_TEST_DIRNAME/wrap.sh"
}

@test "fox-ten" {
  run wrap 'The quick brown fox jumps over the lazy dog' 10
  [ "$output" = $'The quick\nbrown fox\njumps over\nthe lazy\ndog' ]
}

@test "fox-twenty" {
  run wrap 'The quick brown fox jumps over the lazy dog' 20
  [ "$output" = $'The quick brown fox\njumps over the lazy\ndog' ]
}

@test "sentence" {
  run wrap 'Wrap text to a width with a greedy loop.' 15
  [ "$output" = $'Wrap text to a\nwidth with a\ngreedy loop.' ]
}

@test "empty" {
  run wrap '' 10
  [ "$output" = $'' ]
}

@test "only-spaces" {
  run wrap '   ' 5
  [ "$output" = $'' ]
}

@test "fits" {
  run wrap 'short text' 20
  [ "$output" = $'short text' ]
}

@test "exact-fit" {
  run wrap 'abcde fghij' 5
  [ "$output" = $'abcde\nfghij' ]
}

@test "long-word" {
  run wrap 'a supercalifragilistic word' 10
  [ "$output" = $'a\nsupercalifragilistic\nword' ]
}

@test "many-spaces" {
  run wrap 'one   two	three
four' 9
  [ "$output" = $'one two\nthree\nfour' ]
}

@test "width-one" {
  run wrap 'a b c' 1
  [ "$output" = $'a\nb\nc' ]
}

@test "line-exact" {
  run wrap 'ab cd ef' 5
  [ "$output" = $'ab cd\nef' ]
}
