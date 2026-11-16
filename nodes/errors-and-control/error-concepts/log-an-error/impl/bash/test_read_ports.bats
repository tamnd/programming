#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/log-an-error/.

setup() {
  source "$BATS_TEST_DIRNAME/read_ports.sh"
  dir=$(mktemp -d)
}

teardown() {
  rm -rf "${dir:?}"
}

check() { # check WANT_PORTS WANT_LOG TEXT...
  local want_ports=$1 want_log=$2
  shift 2
  read_ports "$@" >"$dir/ports" 2>"$dir/log"
  printf '%s' "$want_ports" >"$dir/want_ports"
  printf '%s' "$want_log" >"$dir/want_log"
  cmp "$dir/ports" "$dir/want_ports" && cmp "$dir/log" "$dir/want_log"
}

@test "one-bad" { check $'8080\n443\n' $'level=ERROR msg="read port failed" line=2 err="not a number: \'x\'"\nlevel=INFO msg="ports read" ok=2 total=3\n' $'8080' $'x' $'443'; }
@test "all-good" { check $'80\n443\n' $'level=INFO msg="ports read" ok=2 total=2\n' $'80' $'443'; }
@test "out-of-range" { check $'' $'level=ERROR msg="read port failed" line=1 err="out of range: 70000"\nlevel=INFO msg="ports read" ok=0 total=1\n' $'70000'; }
@test "empty-list" { check $'' $'level=INFO msg="ports read" ok=0 total=0\n'; }
@test "empty-text" { check $'' $'level=ERROR msg="read port failed" line=1 err="not a number: \'\'"\nlevel=INFO msg="ports read" ok=0 total=1\n' $''; }
@test "boundary" { check $'1\n65535\n' $'level=ERROR msg="read port failed" line=3 err="out of range: 0"\nlevel=INFO msg="ports read" ok=2 total=3\n' $'1' $'65535' $'0'; }
@test "newline" { check $'' $'level=ERROR msg="read port failed" line=1 err="not a number: \'80\\nlevel=INFO msg=\\"all good\\"\'"\nlevel=INFO msg="ports read" ok=0 total=1\n' $'80\nlevel=INFO msg="all good"'; }
@test "backslash" { check $'' $'level=ERROR msg="read port failed" line=1 err="not a number: \'C:\\\\port\'"\nlevel=INFO msg="ports read" ok=0 total=1\n' $'C:\\port'; }
@test "carriage-return" { check $'' $'level=ERROR msg="read port failed" line=1 err="not a number: \'80\\r\'"\nlevel=INFO msg="ports read" ok=0 total=1\n' $'80\r'; }
@test "tab" { check $'' $'level=ERROR msg="read port failed" line=1 err="not a number: \'8\\t0\'"\nlevel=INFO msg="ports read" ok=0 total=1\n' $'8\t0'; }
