#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

setup() {
  source "$BATS_TEST_DIRNAME/letters.sh"
}

@test "hello" {
  run count_letters 'hello'
  [ "$status" -eq 0 ]
  [ "$output" = $'0\n0\n0\n0\n1\n0\n0\n1\n0\n0\n0\n2\n0\n0\n1\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0' ]
}

@test "pangram" {
  run count_letters 'The quick brown fox jumps over the lazy dog'
  [ "$status" -eq 0 ]
  [ "$output" = $'1\n1\n1\n1\n3\n1\n1\n2\n1\n1\n1\n1\n1\n1\n4\n1\n1\n2\n1\n2\n2\n1\n1\n1\n1\n1' ]
}

@test "mississippi" {
  run count_letters 'Mississippi'
  [ "$status" -eq 0 ]
  [ "$output" = $'0\n0\n0\n0\n0\n0\n0\n0\n4\n0\n0\n0\n1\n0\n0\n2\n0\n0\n4\n0\n0\n0\n0\n0\n0\n0' ]
}

@test "empty" {
  run count_letters ''
  [ "$status" -eq 0 ]
  [ "$output" = $'0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0' ]
}

@test "no-letters" {
  run count_letters '123 !?'
  [ "$status" -eq 0 ]
  [ "$output" = $'0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0' ]
}

@test "mixed-case" {
  run count_letters 'AaAa'
  [ "$status" -eq 0 ]
  [ "$output" = $'4\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0' ]
}

@test "first-and-last" {
  run count_letters 'aZ'
  [ "$status" -eq 0 ]
  [ "$output" = $'1\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n1' ]
}

@test "only-z" {
  run count_letters 'zzz'
  [ "$status" -eq 0 ]
  [ "$output" = $'0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n3' ]
}

@test "white-space" {
  run count_letters ' 	
'
  [ "$status" -eq 0 ]
  [ "$output" = $'0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0' ]
}

@test "sentence" {
  run count_letters 'Count Each Letter, once.'
  [ "$status" -eq 0 ]
  [ "$output" = $'1\n0\n3\n0\n4\n0\n0\n1\n0\n0\n0\n1\n0\n2\n2\n0\n0\n1\n0\n3\n1\n0\n0\n0\n0\n0' ]
}
