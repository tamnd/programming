#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

setup() {
  source "$BATS_TEST_DIRNAME/levenshtein.sh"
}

@test "kitten-sitting" {
  run levenshtein 'kitten' 'sitting'
  [ "$status" -eq 0 ]
  [ "$output" = 3 ]
  run levenshtein 'sitting' 'kitten'
  [ "$output" = 3 ]
}

@test "rosettacode" {
  run levenshtein 'rosettacode' 'raisethysword'
  [ "$status" -eq 0 ]
  [ "$output" = 8 ]
  run levenshtein 'raisethysword' 'rosettacode'
  [ "$output" = 8 ]
}

@test "flaw-lawn" {
  run levenshtein 'flaw' 'lawn'
  [ "$status" -eq 0 ]
  [ "$output" = 2 ]
  run levenshtein 'lawn' 'flaw'
  [ "$output" = 2 ]
}

@test "both-empty" {
  run levenshtein '' ''
  [ "$status" -eq 0 ]
  [ "$output" = 0 ]
  run levenshtein '' ''
  [ "$output" = 0 ]
}

@test "empty-left" {
  run levenshtein '' 'abc'
  [ "$status" -eq 0 ]
  [ "$output" = 3 ]
  run levenshtein 'abc' ''
  [ "$output" = 3 ]
}

@test "empty-right" {
  run levenshtein 'abc' ''
  [ "$status" -eq 0 ]
  [ "$output" = 3 ]
  run levenshtein '' 'abc'
  [ "$output" = 3 ]
}

@test "equal" {
  run levenshtein 'same' 'same'
  [ "$status" -eq 0 ]
  [ "$output" = 0 ]
  run levenshtein 'same' 'same'
  [ "$output" = 0 ]
}

@test "one-substitution" {
  run levenshtein 'a' 'b'
  [ "$status" -eq 0 ]
  [ "$output" = 1 ]
  run levenshtein 'b' 'a'
  [ "$output" = 1 ]
}

@test "transposition" {
  run levenshtein 'ab' 'ba'
  [ "$status" -eq 0 ]
  [ "$output" = 2 ]
  run levenshtein 'ba' 'ab'
  [ "$output" = 2 ]
}

@test "case" {
  run levenshtein 'Apple' 'apple'
  [ "$status" -eq 0 ]
  [ "$output" = 1 ]
  run levenshtein 'apple' 'Apple'
  [ "$output" = 1 ]
}

@test "disjoint" {
  run levenshtein 'abc' 'xyz'
  [ "$status" -eq 0 ]
  [ "$output" = 3 ]
  run levenshtein 'xyz' 'abc'
  [ "$output" = 3 ]
}

@test "prefix" {
  run levenshtein 'abc' 'abcdef'
  [ "$status" -eq 0 ]
  [ "$output" = 3 ]
  run levenshtein 'abcdef' 'abc'
  [ "$output" = 3 ]
}

@test "intention" {
  run levenshtein 'intention' 'execution'
  [ "$status" -eq 0 ]
  [ "$output" = 5 ]
  run levenshtein 'execution' 'intention'
  [ "$output" = 5 ]
}

@test "space" {
  run levenshtein 'a b' 'ab'
  [ "$status" -eq 0 ]
  [ "$output" = 1 ]
  run levenshtein 'ab' 'a b'
  [ "$output" = 1 ]
}
