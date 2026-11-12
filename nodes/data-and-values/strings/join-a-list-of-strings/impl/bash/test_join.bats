#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/join-a-list-of-strings/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/join.sh"
}

check() { # check WANT COMMAND ARGS...
  local want=$1
  shift
  run "$@"
  [ "$status" -eq 0 ]
  [ "$output" = "$want" ]
}

@test "comma" { check 'a,b,c' join ',' a b c; }
@test "comma-space" { check 'red, green, blue' join ', ' red green blue; }
@test "words" { check 'Hello world' join ' ' Hello world; }
@test "empty-list" { check '' join ','; }
@test "single" { check 'only' join ',' only; }
@test "empty-items" { check ',,' join ',' '' '' ''; }
@test "empty-middle" { check 'a--b' join '-' a '' b; }
@test "empty-sep" { check 'abc' join '' a b c; }
@test "long-sep" { check 'x -> y' join ' -> ' x y; }
@test "non-ascii" { check 'naïve·日本' join '·' 'naïve' '日本'; }
@test "astral-sep" { check 'a🙂b' join '🙂' a b; }
@test "sep-in-item" { check 'a,b,c' join ',' 'a,b' c; }
@test "numbers" { check '1,2,3' join_numbers ',' 1 2 3; }
@test "negative" { check '-1 0 1' join_numbers ' ' -1 0 1; }
@test "empty-numbers" { check '' join_numbers ','; }
@test "single-number" { check '42' join_numbers ',' 42; }
@test "int32-ends" { check '2147483647;-2147483648' join_numbers ';' 2147483647 -2147483648; }
