#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-text-template/.

setup() {
  source "$BATS_TEST_DIRNAME/template.sh"
}

@test "greeting" {
  run substitute 'Hello, $name!' 'name' 'Ada'
  [ "$status" -eq 0 ]
  [ "$output" = 'Hello, Ada!' ]
}

@test "braces" {
  run substitute '${item}s cost $$${price}' 'item' 'Apple' 'price' '3'
  [ "$status" -eq 0 ]
  [ "$output" = 'Apples cost $3' ]
}

@test "two-uses" {
  run substitute '$a and $a again' 'a' 'x'
  [ "$status" -eq 0 ]
  [ "$output" = 'x and x again' ]
}

@test "no-placeholders" {
  run substitute 'plain text' 
  [ "$status" -eq 0 ]
  [ "$output" = 'plain text' ]
}

@test "empty-template" {
  run substitute '' 'x' '1'
  [ "$status" -eq 0 ]
  [ "$output" = '' ]
}

@test "dollar-escape" {
  run substitute '$$name' 'name' 'Ada'
  [ "$status" -eq 0 ]
  [ "$output" = '$name' ]
}

@test "adjacent-text" {
  run substitute '${n}th' 'n' '4'
  [ "$status" -eq 0 ]
  [ "$output" = '4th' ]
}

@test "value-with-dollar" {
  run substitute '$v' 'v' '$x and ${y}'
  [ "$status" -eq 0 ]
  [ "$output" = '$x and ${y}' ]
}

@test "empty-value" {
  run substitute '[$v]' 'v' ''
  [ "$status" -eq 0 ]
  [ "$output" = '[]' ]
}

@test "underscore-name" {
  run substitute '$first_name $last_name' 'first_name' 'Grace' 'last_name' 'Hopper'
  [ "$status" -eq 0 ]
  [ "$output" = 'Grace Hopper' ]
}

@test "name-ends-at-punct" {
  run substitute '$a.$b' 'a' '1' 'b' '2'
  [ "$status" -eq 0 ]
  [ "$output" = '1.2' ]
}

@test "unused-value" {
  run substitute '$a' 'a' '1' 'b' '2'
  [ "$status" -eq 0 ]
  [ "$output" = '1' ]
}

@test "unknown-name" {
  run substitute 'Hi $who' name Ada
  [ "$status" -eq 1 ]
}
