#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-stack/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/balanced.sh"
}

check() { # check STATUS TEXT
  run balanced "$2"
  [ "$status" -eq "$1" ]
  [ "$output" = "" ]
}

deep() {
  local open close
  printf -v open '%1000s' ''
  printf -v close '%1000s' ''
  printf '%s%s' "${open// /(}" "${close// /)}"
}

@test "pair" {
  check 0 '()'
}

@test "nested" {
  check 0 '{[()()]}'
}

@test "code" {
  check 0 'f(a[0], {b: 1})'
}

@test "crossed" {
  check 1 '([)]'
}

@test "empty" {
  check 0 ''
}

@test "no-brackets" {
  check 0 'abc'
}

@test "unclosed" {
  check 1 '(('
}

@test "extra-close" {
  check 1 '())'
}

@test "close-first" {
  check 1 ')('
}

@test "wrong-kind" {
  check 1 '(]'
}

@test "deep" {
  check 0 "$(deep)"
}
