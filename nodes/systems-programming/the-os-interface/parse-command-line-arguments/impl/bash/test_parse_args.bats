#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-command-line-arguments/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/parse_args.sh"
}

check() { # check VERBOSE LIMIT COUNT FILE... ARG...: COUNT is the number of FILEs
  local want_verbose=$1 want_limit=$2
  local -a want_files=("${@:4:$3}")
  shift $((3 + $3))
  parse_args "$@"
  [ "$verbose" = "$want_verbose" ]
  [ "$limit" = "$want_limit" ]
  [ "${#files[@]}" -eq "${#want_files[@]}" ]
  local i
  for i in "${!want_files[@]}"; do
    [ "${files[i]}" = "${want_files[i]}" ]
  done
}

rejects() { # rejects ARG...
  run -2 parse_args "$@"
}

@test "files-only" { check false 10 2 a.txt b.txt a.txt b.txt; }
@test "short-options" { check true 3 1 a.txt -v -n 3 a.txt; }
@test "long-options" { check true 5 2 a.txt b.txt --verbose --limit 5 a.txt b.txt; }
@test "no-args" { check false 10 0; }
@test "long-equals" { check false 7 1 notes.md --limit=7 notes.md; }
@test "repeated" { check false 5 1 a.txt -n 3 --limit 5 a.txt; }
@test "zero" { check false 0 1 a.txt -n 0 a.txt; }
@test "end-of-options" { check true 10 2 -n --limit=4 -v -- -n --limit=4; }
@test "only-terminator" { check false 10 0 --; }
@test "second-terminator" { check false 10 1 -- -- --; }
@test "dash" { check false 10 1 - -; }
@test "dash-after-option" { check false 2 2 - b.txt -n 2 - b.txt; }
@test "empty-arg" { check false 10 1 "" ""; }
@test "space" { check true 10 1 "my file.txt" -v "my file.txt"; }
@test "non-ascii" { check false 4 2 日本語.txt café.md --limit=4 日本語.txt café.md; }

@test "unknown-short" { rejects -x a.txt; }
@test "unknown-long" { rejects --color a.txt; }
@test "missing-value" { rejects -n; }
@test "missing-value-long" { rejects --limit; }
@test "not-a-number" { rejects -n ten a.txt; }
@test "not-a-number-long" { rejects --limit=ten; }
@test "empty-value" { rejects --limit= a.txt; }
@test "verbose-with-value" { rejects --verbose=yes; }
