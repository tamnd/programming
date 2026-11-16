#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-custom-error-type/.

setup() {
  source "$BATS_TEST_DIRNAME/check_port.sh"
}

check() { # check TEXT WANT
  local got
  got=$(check_port "$1")
  [ "$got" = "$2" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$2"; return 1; }
}

@test "valid" { check "8080" "port 8080"; }
@test "not-a-number" { check "x80" "invalid port 'x80': not a number"; }
@test "too-large" { check "65536" "invalid port '65536': out of range"; }
@test "empty" { check "" "invalid port '': empty"; }
@test "zero" { check "0" "invalid port '0': out of range"; }
@test "one" { check "1" "port 1"; }
@test "largest" { check "65535" "port 65535"; }
@test "leading-zeros" { check "0080" "port 80"; }
@test "negative" { check "-1" "invalid port '-1': not a number"; }
@test "leading-space" { check " 80" "invalid port ' 80': not a number"; }
@test "overflow" { check "99999999999999999999" "invalid port '99999999999999999999': out of range"; }

@test "fields" {
  parse_port x80 || true
  [ "$PORT_ERROR_TEXT" = x80 ]
  [ "$PORT_ERROR_REASON" = "not a number" ]
}
