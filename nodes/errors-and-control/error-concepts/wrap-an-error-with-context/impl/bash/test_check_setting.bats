#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wrap-an-error-with-context/.

setup() {
  source "$BATS_TEST_DIRNAME/check_setting.sh"
}

check() { # check WHERE TEXT WANT
  local got
  got=$(check_setting "$1" "$2")
  [ "$got" = "$3" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$3"; return 1; }
}

@test "valid" { check "app.conf:3" "8080" "port 8080"; }
@test "not-a-number" { check "app.conf:3" "x" "error: app.conf:3: read port: not a number: 'x'"; }
@test "too-large" { check "app.conf:7" "70000" "error: app.conf:7: read port: out of range: 70000"; }
@test "empty" { check "app.conf:1" "" "error: app.conf:1: read port: not a number: ''"; }
@test "zero" { check "app.conf:2" "0" "error: app.conf:2: read port: out of range: 0"; }
@test "largest" { check "app.conf:2" "65535" "port 65535"; }
@test "leading-space" { check "app.conf:4" " 80" "error: app.conf:4: read port: not a number: ' 80'"; }
@test "overflow" { check "app.conf:5" "99999999999999999999" "error: app.conf:5: read port: out of range: 99999999999999999999"; }
@test "negative" { check "app.conf:6" "-1" "error: app.conf:6: read port: not a number: '-1'"; }

@test "cause" {
  local status=0
  read_port app.conf:3 x || status=$?
  [ "$status" -eq 1 ]
}
