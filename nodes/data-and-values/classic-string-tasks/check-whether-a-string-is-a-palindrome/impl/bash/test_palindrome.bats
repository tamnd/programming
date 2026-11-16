#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

setup() {
  source "$BATS_TEST_DIRNAME/palindrome.sh"
}

@test "racecar" {
  run is_palindrome 'racecar'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "panama" {
  run is_palindrome 'A man, a plan, a canal: Panama'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "hello" {
  run is_palindrome 'hello'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
}

@test "empty" {
  run is_palindrome ''
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "only-punctuation" {
  run is_palindrome '?! ...'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "one-letter" {
  run is_palindrome 'a'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "two-letters" {
  run is_palindrome 'ab'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
}

@test "mixed-case" {
  run is_palindrome 'Aa'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "nixon" {
  run is_palindrome 'No '\''x'\'' in Nixon'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "cat-i-saw" {
  run is_palindrome 'Was it a car or a cat I saw?'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "digits" {
  run is_palindrome '12321'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
}

@test "digits-not" {
  run is_palindrome '123'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
}

@test "zero-p" {
  run is_palindrome '0P'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
}

@test "race-a-car" {
  run is_palindrome 'race a car'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
}

@test "even-near" {
  run is_palindrome 'abca'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
}
