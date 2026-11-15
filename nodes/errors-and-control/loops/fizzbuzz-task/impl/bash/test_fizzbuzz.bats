#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/fizzbuzz-task/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/fizzbuzz.sh"
}

check() { # check N WANT...
  local n=$1
  shift
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(fizzbuzz "$n"; printf x)
  want=$(if (($# > 0)); then printf '%s\n' "$@"; fi; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "fifteen" { check 15 1 2 Fizz 4 Buzz Fizz 7 8 Fizz Buzz 11 Fizz 13 14 FizzBuzz; }
@test "thirty" { check 30 1 2 Fizz 4 Buzz Fizz 7 8 Fizz Buzz 11 Fizz 13 14 FizzBuzz 16 17 Fizz 19 Buzz Fizz 22 23 Fizz Buzz 26 Fizz 28 29 FizzBuzz; }
@test "zero" { check 0; }
@test "one" { check 1 1; }
@test "three" { check 3 1 2 Fizz; }
@test "five" { check 5 1 2 Fizz 4 Buzz; }
