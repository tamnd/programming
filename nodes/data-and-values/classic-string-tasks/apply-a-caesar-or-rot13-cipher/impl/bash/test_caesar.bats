#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

setup() {
  source "$BATS_TEST_DIRNAME/caesar.sh"
}

@test "hi-2" {
  run caesar 'HI' 2
  [ "$status" -eq 0 ]
  [ "$output" = 'JK' ]
}

@test "hi-20" {
  run caesar 'HI' 20
  [ "$status" -eq 0 ]
  [ "$output" = 'BC' ]
}

@test "rot13" {
  run caesar 'Hello, World!' 13
  [ "$status" -eq 0 ]
  [ "$output" = 'Uryyb, Jbeyq!' ]
}

@test "empty" {
  run caesar '' 3
  [ "$status" -eq 0 ]
  [ "$output" = '' ]
}

@test "zero" {
  run caesar 'abc' 0
  [ "$status" -eq 0 ]
  [ "$output" = 'abc' ]
}

@test "full-turn" {
  run caesar 'abc' 26
  [ "$status" -eq 0 ]
  [ "$output" = 'abc' ]
}

@test "minus-one" {
  run caesar 'abc' -1
  [ "$status" -eq 0 ]
  [ "$output" = 'zab' ]
}

@test "decode" {
  run caesar 'Uryyb, Jbeyq!' -13
  [ "$status" -eq 0 ]
  [ "$output" = 'Hello, World!' ]
}

@test "above-26" {
  run caesar 'xyz' 27
  [ "$status" -eq 0 ]
  [ "$output" = 'yza' ]
}

@test "wrap" {
  run caesar 'Zz' 1
  [ "$status" -eq 0 ]
  [ "$output" = 'Aa' ]
}

@test "non-letters" {
  run caesar '123 !? [`{@' 5
  [ "$status" -eq 0 ]
  [ "$output" = '123 !? [`{@' ]
}

@test "non-ascii" {
  run caesar 'café ü' 1
  [ "$status" -eq 0 ]
  [ "$output" = 'dbgé ü' ]
}

@test "large" {
  run caesar 'abc' 9007199254740991
  [ "$status" -eq 0 ]
  [ "$output" = 'fgh' ]
}

@test "large-negative" {
  run caesar 'abc' -9007199254740991
  [ "$status" -eq 0 ]
  [ "$output" = 'vwx' ]
}
