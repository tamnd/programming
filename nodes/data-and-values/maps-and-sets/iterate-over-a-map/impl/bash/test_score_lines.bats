#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-a-map/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/score_lines.sh"
}

check() { # check WANT ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(score_lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "scores" { check "amy=9\nbob=7\ncal=4\ntotal=20\n" bob=7 amy=9 cal=4; }
@test "one" { check "x=1\ntotal=1\n" x=1; }
@test "empty" { check "total=0\n"; }
@test "case" { check "A=4\nB=2\na=3\nb=1\ntotal=10\n" b=1 B=2 a=3 A=4; }
@test "negative" { check "a=-5\nb=5\ntotal=0\n" a=-5 b=5; }
@test "prefix" { check "a=2\nab=1\nabc=3\ntotal=6\n" abc=3 a=2 ab=1; }
@test "reverse" { check "a=10\nb=9\nc=8\nd=7\ne=6\nf=5\ng=4\nh=3\ni=2\nj=1\ntotal=55\n" j=1 i=2 h=3 g=4 f=5 e=6 d=7 c=8 b=9 a=10; }
