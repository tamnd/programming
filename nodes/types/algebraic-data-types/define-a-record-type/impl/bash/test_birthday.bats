#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-record-type/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/birthday.sh"
}

check() { # check WANT NAME PERSON...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(birthday "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "first" { check "Ada|37\nAlan|41\n" Ada 'Ada|36' 'Alan|41'; }
@test "second" { check "Ada|36\nAlan|42\n" Alan 'Ada|36' 'Alan|41'; }
@test "empty" { check "" Ada; }
@test "not-found" { check "Ada|36\n" Grace 'Ada|36'; }
@test "duplicate-name" { check "Ada|37\nAlan|41\nAda|10\n" Ada 'Ada|36' 'Alan|41' 'Ada|9'; }
@test "zero-age" { check "Baby|1\n" Baby 'Baby|0'; }
@test "non-ascii" { check "Zoë|21\nZoe|30\n" Zoë 'Zoë|20' 'Zoe|30'; }
@test "case" { check "ada|36\n" Ada 'ada|36'; }
