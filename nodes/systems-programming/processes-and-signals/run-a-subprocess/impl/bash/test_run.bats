#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/run-a-subprocess/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/run.sh"
}

check() { # check STATUS OUT ERR PROGRAM [ARG...]
  local want_status=$1 want_out=$2 want_err=$3 status out err
  shift 3
  run_program status out err "$@"
  [ "$status" = "$want_status" ]
  [ "$out" = "$want_out" ]
  [ "$err" = "$want_err" ]
}

nl=$'\n'

@test "hello" { check 0 "hello" "" printf hello; }
@test "two-arguments" { check 0 "one two$nl" "" echo one two; }
@test "exit-status" { check 3 "" "" sh -c 'exit 3'; }
@test "both-streams" { check 2 "out$nl" "err$nl" sh -c 'echo out; echo err >&2; exit 2'; }
@test "metacharacters" { check 0 "a b|\$HOME|*|x;y|\"q\"|'s'|" "" printf '%s|' 'a b' '$HOME' '*' 'x;y' '"q"' "'s'"; }
@test "empty-argument" { check 0 "[]" "" printf '[%s]' ''; }
@test "newline-argument" { check 0 "line one${nl}line two" "" printf '%s' "line one${nl}line two"; }
@test "trailing-newlines" { check 0 "a$nl$nl" "" printf 'a\n\n'; }
@test "non-ascii" { check 0 "xin chào, 日本語, 🙂$nl" "" printf '%s\n' 'xin chào, 日本語, 🙂'; }
@test "stderr-only" { check 0 "" "warning$nl" sh -c 'echo warning >&2'; }
@test "no-output" { check 0 "" "" true; }
@test "false" { check 1 "" "" false; }
@test "status-255" { check 255 "" "" sh -c 'exit 255'; }
@test "empty-stdin" { check 0 "eof$nl" "" sh -c 'if read -r line; then echo got; else echo eof; fi'; }

@test "large-output-on-both-streams" {
  local status out err
  run_program status out err sh -c "head -c 1048576 /dev/zero | tr '\\0' x; head -c 1048576 /dev/zero | tr '\\0' y >&2"
  [ "$status" -eq 0 ]
  [ "$out" = "$(head -c 1048576 /dev/zero | tr '\0' x)" ]
  [ "$err" = "$(head -c 1048576 /dev/zero | tr '\0' y)" ]
}

@test "missing-program" {
  run --separate-stderr run_program s o e no-such-program-7f3a
  [ "$status" -eq 1 ]
  [ -z "$output" ]
  [ -n "$stderr" ]
}
