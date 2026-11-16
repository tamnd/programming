#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-anagrams/.

setup() {
  source "$BATS_TEST_DIRNAME/anagrams.sh"
}

@test "listen-silent" {
  run are_anagrams 'listen' 'silent'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams 'silent' 'listen'
  [ "$output" = true ]
}

@test "dormitory" {
  run are_anagrams 'Dormitory' 'dirty room'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams 'dirty room' 'Dormitory'
  [ "$output" = true ]
}

@test "hello-world" {
  run are_anagrams 'hello' 'world'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
  run are_anagrams 'world' 'hello'
  [ "$output" = false ]
}

@test "both-empty" {
  run are_anagrams '' ''
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams '' ''
  [ "$output" = true ]
}

@test "one-empty" {
  run are_anagrams 'a' ''
  [ "$status" -eq 0 ]
  [ "$output" = false ]
  run are_anagrams '' 'a'
  [ "$output" = false ]
}

@test "same-set-other-counts" {
  run are_anagrams 'aab' 'abb'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
  run are_anagrams 'abb' 'aab'
  [ "$output" = false ]
}

@test "extra-letter" {
  run are_anagrams 'abc' 'abcd'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
  run are_anagrams 'abcd' 'abc'
  [ "$output" = false ]
}

@test "same-length-other-letters" {
  run are_anagrams 'abc' 'abd'
  [ "$status" -eq 0 ]
  [ "$output" = false ]
  run are_anagrams 'abd' 'abc'
  [ "$output" = false ]
}

@test "case" {
  run are_anagrams 'Listen' 'SILENT'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams 'SILENT' 'Listen'
  [ "$output" = true ]
}

@test "same-word" {
  run are_anagrams 'stop' 'stop'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams 'stop' 'stop'
  [ "$output" = true ]
}

@test "gentleman" {
  run are_anagrams 'a gentleman' 'elegant man'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams 'elegant man' 'a gentleman'
  [ "$output" = true ]
}

@test "they-see" {
  run are_anagrams 'The eyes!' 'They see.'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams 'They see.' 'The eyes!'
  [ "$output" = true ]
}

@test "punctuation-only" {
  run are_anagrams '!?' '...'
  [ "$status" -eq 0 ]
  [ "$output" = true ]
  run are_anagrams '...' '!?'
  [ "$output" = true ]
}
